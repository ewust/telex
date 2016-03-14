#include <event2/dns.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/listener.h>
#include <event2/event.h>
#include <event2/bufferevent_ssl.h>
#include <openssl/ssl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "logger.h"
#include "proxy.h"
#include "state.h"
#include "util.h"
#include "ssl.h"

#define MAX_OUTPUT_BUFFER (512*1024)


void ssl_accept_cb(struct telex_state *state)
{    
    unsigned char context[MAX_CONTEXT_LEN];
    
    // Construct context string
    // client_random is 4 bytes of timestamp, 28 bytes random nonce
    uint32_t local_addr = get_local_addr(bufferevent_getfd(state->ssl_sock));
    memset(context, 0, MAX_CONTEXT_LEN);
    memcpy(&context[0], &local_addr, sizeof(uint32_t));
    memcpy(&context[4], state->ssl->s3->client_random, 4);

    state->service = bufferevent_socket_new(state->base, -1, BEV_OPT_CLOSE_ON_FREE);
  
    // Setup (local) service TCP to either the tagged or untagged port
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (!check_tag(state->secret, state->conf->privkey,
                   &state->ssl->s3->client_random[4], context, MAX_CONTEXT_LEN)) {
        //tagged
        sin.sin_port = htons(state->conf->tag_port);
    } else {
        //untagged
        sin.sin_port = htons(state->conf->untag_port);
    }
    
    bufferevent_socket_connect(state->service, (struct sockaddr*)&sin, sizeof(sin));
    
    bufferevent_setcb(state->service, (bufferevent_data_cb)read_cb, NULL,
        (bufferevent_event_cb)event_cb, state); 
}


struct telex_state *StateInit(struct telex_conf *conf)
{
    struct telex_state *state;
    state = calloc(1, sizeof(struct telex_state));
    assert(state);
    state->conf = conf;
    state->id = conf->total_connections++;
	state->conf->count_open_tunnels++;
    state->start_ms = time_ms();
    snprintf(state->name, sizeof(state->name), "tunnel %d", state->id);
    return state;
}

// Deallocate dynamic structures, close socket,
// and free State object itself.
// Please add cleanup code here if you extend
// the structure!
void StateCleanup(struct telex_state **_state)
{
	if (!_state || !_state)
		return;	
	struct telex_state *state = *_state;
	*_state = NULL;

	if (state->service) {
		//_dec(BEV, state->local);
		bufferevent_free(state->service);
		state->service = NULL;
	}
	if (state->client) {
		//_dec(BEV, state->remote);
		bufferevent_free(state->client);
		state->client = NULL;
	}
	if (state->ssl) {
		//_dec(SSL, state->ssl);
		SSL_free(state->ssl);
		state->ssl = NULL;
	}
	if (state->ssl_sock) {
		//_dec(BEV, state->remotetcp);
		bufferevent_free(state->ssl_sock);
		state->ssl_sock = NULL;
	}

	// TODO: Do we have to close the sockets?	

	state->conf->count_open_tunnels--;
	unsigned long long duration = time_ms() - state->start_ms;
	LogInfo(state->name, "Closed (%u active); %ld up  %ld down  %0.3f s",
		state->conf->count_open_tunnels,
		state->in_local, state->in_remote,
		duration/1000.);
	//_dec(STATE, state); 
    free(state);
}

// TODO: make all this notblocking/event driven
void tcp_accept_cb(struct evconnlistener *listener, evutil_socket_t fd,
                     struct sockaddr *address, int socklen,
                     struct telex_conf *conf)
{
    BIO *sbio;
    struct telex_state *state = StateInit(conf);
	state->base = evconnlistener_get_base(listener);

    state->ssl_sock = bufferevent_socket_new(state->base, fd,
        BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);



    sbio = BIO_new_socket(fd, BIO_NOCLOSE);
    state->ssl = SSL_new(conf->ssl_ctx);
    if (!state->ssl) {
        LogError("ssl", "Could not create SSL object");
        return;
    }
    state->client = bufferevent_openssl_filter_new(state->base, state->ssl_sock,
        state->ssl, BUFFEREVENT_SSL_ACCEPTING, BEV_OPT_DEFER_CALLBACKS);

    bufferevent_setcb(state->client, (bufferevent_data_cb)read_cb, NULL,
        (bufferevent_event_cb)event_cb, state);
}

#define ISLOCAL(bev, state) ((bev) == (state)->service)
#define ISREMOTE(bev, state) ((bev) == (state)->client)
#define PARTY(bev, state) (ISLOCAL((bev),(state)) ? "service" : \
	(ISREMOTE((bev),(state)) ? "client" : "other" ))

void read_cb(struct bufferevent *bev, struct telex_state *state)
{
	struct bufferevent *partner = 
			ISLOCAL(bev,state) ? state->client : state->service;

	struct evbuffer *src = bufferevent_get_input(bev);
	size_t len = evbuffer_get_length(src);
	size_t total = 0;
	if (ISLOCAL(bev,state)) {
		total = (state->in_local += len);
	} else if (ISREMOTE(bev,state)) {
		total = (state->in_remote += len);
	} else {
		assert(0);
	}

	LogTrace(state->name, "READCB %s (got %lu bytes / %lu so far)", 
		PARTY(bev,state), (unsigned long)len, total);

	if (!partner) {
		LogTrace(state->name, "Partner missing; discarding data");
		evbuffer_drain(src, len);
		return;
	}

	struct evbuffer *dst = bufferevent_get_output(partner);		
	evbuffer_add_buffer(dst, src); // copy from input to output

	if (evbuffer_get_length(dst) >= MAX_OUTPUT_BUFFER) {
		LogDebug(state->name, "PAUSING (dst: %d bytes)", 
			evbuffer_get_length(dst));
		bufferevent_setcb(partner,
			(bufferevent_data_cb)read_cb, 
			(bufferevent_data_cb)drained_write_cb,
			(bufferevent_event_cb)event_cb, state);
		bufferevent_setwatermark(partner, EV_WRITE, 
				MAX_OUTPUT_BUFFER/2, MAX_OUTPUT_BUFFER);
		bufferevent_disable(bev, EV_READ);
	}
}

void drained_write_cb(struct bufferevent *bev, struct telex_state *state)
{
	LogDebug(state->name, "DRAINING %s", PARTY(bev,state));
	struct bufferevent *partner = 
			ISLOCAL(bev,state) ? state->client : state->service;
	bufferevent_setcb(bev, (bufferevent_data_cb)read_cb, NULL, 
		(bufferevent_event_cb)event_cb, state);
	bufferevent_setwatermark(bev, EV_WRITE, 0, 0);
	if (partner) {
		bufferevent_enable(partner, EV_READ);
	}
}

void close_on_finished_write_cb(struct bufferevent *bev, struct telex_state *state)
{
	struct evbuffer *b = bufferevent_get_output(bev);
	LogDebug(state->name, "CLOSE_ON_FINISHED %s (%d bytes remaining)", 
		PARTY(bev,state), evbuffer_get_length(b));
	if (evbuffer_get_length(b) == 0) {
		StateCleanup(&state);
	}
}

static void show_connection_error(struct bufferevent *bev, struct telex_state *state)
{
	int messages = 0;
	unsigned long err;
	
	messages += ssl_log_errors(LOG_ERROR, state->name);
	while ((err = bufferevent_socket_get_dns_error(bev))) {
		LogError(state->name, "DNS error: %s", evutil_gai_strerror(err));
		messages++;
	}
	if (!messages) {
		if (errno) {
			LogError(state->name, "Connection error: %s", strerror(errno));
		} else {
			LogError(state->name, "Connection error");
		}
	}
}

void event_cb(struct bufferevent *bev, short events, struct telex_state *state)
{	
	struct bufferevent *partner = 
			ISLOCAL(bev,state) ? state->client : state->service;

	if (events & BEV_EVENT_CONNECTED) {
		LogTrace(state->name, "EVENT_CONNECTED %s", PARTY(bev,state));
		LogTrace(state->name, "SSL state: %s", SSL_state_string_long(state->ssl));
        if (!state->service) {
            ssl_accept_cb(state);
        }
		bufferevent_enable(state->service,  EV_READ|EV_WRITE);
		bufferevent_enable(state->client, EV_READ|EV_WRITE);
		return;

	} else if (events & BEV_EVENT_EOF) {
		LogTrace(state->name, "EVENT_EOF %s", PARTY(bev,state));
		if (partner) {
			// flush pending data
			if (evbuffer_get_length(bufferevent_get_input(bev))) {
				read_cb(bev, state);
			}			
			if (evbuffer_get_length(bufferevent_get_output(partner))) {
				// output still pending
				bufferevent_setcb(partner, NULL, 
					(bufferevent_data_cb)close_on_finished_write_cb,
					(bufferevent_event_cb)event_cb, state);
				bufferevent_disable(partner, EV_READ);
				return;
			}
		}
		StateCleanup(&state);
		return;

	} else if (events & BEV_EVENT_ERROR) {
		LogTrace(state->name, "EVENT_ERROR %s", PARTY(bev,state));
		show_connection_error(bev, state);
		StateCleanup(&state);
		return;
	}
}





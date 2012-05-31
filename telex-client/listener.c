#include <event2/dns.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#ifdef __FreeBSD__
#include <arpa/inet.h>  
#include <netinet/in.h> 
#endif

#include "listener.h"
#include "util.h"
#include "logger.h"
#include "state.h"

static void sigint_cb(evutil_socket_t sig, short events, void *user_data);
static void listener_default_error_cb(struct evconnlistener *listener, void *ctx);

// Initializes TCP and LibEvent, then listens for connections on port
// and calls accept_cb when they occur.  Terminates cleanly on SIGINT.
// Returns 0 on clean shutdown; otherwise nonzero
int InitAndListenLoop(int port, evconnlistener_cb accept_cb, struct telex_conf *conf)
{
#ifdef WIN32
	WSADATA WSAData;
	WSAStartup(0x101, &WSAData);
#endif
	
	struct event_base *base;
	base = event_base_new();
	if (!base) {
		LogFatal("listener", "Could not initialize libevent");
		return 1;
	}

	conf->dns_base = evdns_base_new(base, 1);
	if (!conf->dns_base) {
		LogFatal("listener", "Could not initialize dns");
		return 1;
	}

	struct evconnlistener *listener;
	listener = listener_init_local(base, port, accept_cb, conf, NULL);
	if (!listener) {
		LogFatal("listener", "Could not create listener");
		return 1;
	}

	struct event *signal_event;
	signal_event = evsignal_new(base, SIGINT, sigint_cb, (void *)base);
	if (!signal_event || event_add(signal_event, NULL)<0) {
		LogFatal("listener", "Could not create/add signal event");
		return 1;
	}

	// handle events ...
	LogTrace("listener", "Starting dispatch");
	event_base_dispatch(base);

	evconnlistener_free(listener);
	event_free(signal_event);
	event_base_free(base);

	LogInfo("listener", "Shutdown complete");
	return 0;
}

// Initializes event-driven connection listener on 127.0.0.1:port,
// with given accept and error callbacks.
// Returns pointer to listener or 0 on error
struct evconnlistener *listener_init_local(struct event_base *base, int port, 
                                           evconnlistener_cb accept_cb, void *state,
                                           evconnlistener_errorcb error_cb /* optional */)
{
	if (error_cb == NULL) {
		error_cb = listener_default_error_cb;
	}

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(0x7f000001); // i.e., 127.0.0.1
	sin.sin_port = htons(port);

	struct evconnlistener *listener;
	listener = evconnlistener_new_bind(base, accept_cb, state,
		LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, -1,
		(struct sockaddr*)&sin, sizeof(sin));
	if (!listener) {
		return 0;
    }
	evconnlistener_set_error_cb(listener, error_cb);

	char addr[255];
	address_to_string(&sin, addr, sizeof(addr));
	LogInfo("listener", "Listening on %s", addr);

	return listener;
}

// Shutdown on sigint
static void sigint_cb(evutil_socket_t sig, short events, void *user_data)
{
    (void) sig;
    (void) events;   // Avoid warning about unused parameter

	struct event_base *base = user_data;
	LogInfo("listener", "Got interrupt signal");
	event_base_loopexit(base, NULL);
}

// Shutdown on listener error
static void listener_default_error_cb(struct evconnlistener *listener, void *ctx)
{
    (void) ctx;     // Avoid warning about unused parameter

	struct event_base *base = evconnlistener_get_base(listener);
	int err = EVUTIL_SOCKET_ERROR();
	LogFatal("listener", "Got an error %d (%s) on the listener; shutting down", 
		err, evutil_socket_error_to_string(err));
	event_base_loopexit(base, NULL);
}

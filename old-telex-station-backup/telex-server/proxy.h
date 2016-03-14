#ifndef _PROXY_H
#define _PROXY_H

#include "state.h"
#include "logger.h"
#include "listener.h"

void ssl_accept_cb(struct telex_state *state);
void tcp_accept_cb(struct evconnlistener *listener, evutil_socket_t fd,
                     struct sockaddr *address, int socklen,
                     struct telex_conf *conf);

void read_cb(struct bufferevent *bev, struct telex_state *state);
void drained_write_cb(struct bufferevent *bev, struct telex_state *state);
void close_on_finished_write_cb(struct bufferevent *bev, struct telex_state *state);
void event_cb(struct bufferevent *bev, short events, struct telex_state *state);

#endif

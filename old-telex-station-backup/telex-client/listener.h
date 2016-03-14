#ifndef _LISTENER_H_
#define _LISTENER_H_

#include <event2/listener.h>
#include <event2/event.h>

#include "state.h"

#if !defined(LIBEVENT_VERSION_NUMBER) || LIBEVENT_VERSION_NUMBER < 0x02000100
#error "This version of Libevent is not supported; Get 2.0.1-alpha or later."
#endif

// Initializes TCP and LibEvent, then listens for connections on port
// and calls accept_cb when they occur.  Terminates cleanly on SIGINT.
// Returns 0 on clean shutdown; otherwise nonzero
int InitAndListenLoop(int port, evconnlistener_cb accept_cb, struct telex_conf *conf);

// Initializes event-driven connection listener on 127.0.0.1:port,
// with given accept and error callbacks.
// Returns pointer to listener or 0 on error
struct evconnlistener *listener_init_local(struct event_base *base, int port,
    evconnlistener_cb accept_cb, void *state, evconnlistener_errorcb error_cb);

#endif//_LISTENER_H_

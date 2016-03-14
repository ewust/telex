#ifndef _STATE_H_
#define _STATE_H_

#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/dns.h>
#include <openssl/ssl.h>
#include "tag/tag.h"

#if !defined(LIBEVENT_VERSION_NUMBER) || LIBEVENT_VERSION_NUMBER < 0x02000100
#error "This version of Libevent is not supported; Get 2.0.1-alpha or later."
#endif


// Global configuration
struct telex_conf {

    // certificate chain file
    const char *keyfile;
    const char *dh_file;
    byte  privkey[PTWIST_BYTES]; 

	struct evdns_base *dns_base;
    SSL_CTX *ssl_ctx;

    int tag_port;
    int untag_port;
    
    int total_connections;
    int count_open_tunnels;
};

struct telex_state {
    char name[64];
    int id;
    // Global configuration
    struct telex_conf *conf;

    struct bufferevent *ssl_sock; // Underlying socket for SSL
    SSL *ssl;
    
    struct event_base *base;

	unsigned long long start_ms;

    struct bufferevent *client;     // remote
    struct bufferevent *service;    // local


    uint64_t in_local;
    uint64_t in_remote;


    byte secret[16]; // Not used, but hey why not store secrets that can 
            // be used to recover the master secret for the SSL connection?
            // What could possibly go wrong?
 
};



#endif

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

typedef byte Tag[PTWIST_TAG_BYTES];
typedef byte Secret[16];

// Global state and configuration
struct telex_conf
{
// configuration:

	// notblocked location
	const char *notblocked_host;
	int notblocked_port;

	// crypto param files
	char *ca_list;
	char *dh_file;

	// public key filename
	const char *keyfile;

	// global state:
	struct evdns_base *dns_base;
	SSL_CTX *ssl_ctx;

	// connection counters
	unsigned int count_tunnels;
	unsigned int count_open_tunnels;

};

// Connection-oriented state
struct telex_state
{
	struct event_base *base;

	// tunnel index and start time
	unsigned int id;
	unsigned long long start_ms;
	char name[64];

	// reference to global configuration
	struct telex_conf *conf;

	// libevent objects that wrap the sockets and their events
	struct bufferevent *local;
	struct bufferevent *remote;
	struct bufferevent *remotetcp; // (underlies SSL filter)

	// count bytes read from local and remote hosts
	size_t in_local;
	size_t in_remote;

	// SSL connection object and Telex crypto state
	SSL *ssl;
	Tag tag;
	Secret secret;	
};

#endif//_STATE_H_

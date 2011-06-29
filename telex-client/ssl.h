#ifndef _TELEXSSL_H_
#define _TELEXSSL_H_

#include <openssl/ssl.h>
#include <openssl/err.h>

#include "logger.h"
#include "state.h"

// Initialize SSL engine based on settings from conf
int ssl_init(struct telex_conf *conf);

// Log any queued SSL errors from the current thread
// at the given LogLevel
int ssl_log_errors(enum LogLevel level, const char *name);

// Clean up SSL contexts
void ssl_done(struct telex_conf *conf);

// Creates a new SSL connection objected in state->ssl and
// initializes it for a Telex connection.
// Returns 0 on successful Telex initialization; nonzero otherwise.
int ssl_new_telex(struct telex_state *state, unsigned long server_ip);

// Performs post-connection Telex stage on connection state->ssl.
// Returns 0 on successful Telex connection; nonzero otherwise.
int ssl_connected_telex(struct telex_state *state);
	
#endif//_TELEXSSL_H_

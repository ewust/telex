#ifndef LIBTELEXTUNNEL_H
#define LIBTELEXTUNNEL_H

#include <openssl/ssl.h>
#include "tag.h"

struct TelexTunnel_st {
  SSL *ssl;
  int sock;
  byte shared_secret[16];
  byte tag[PTWIST_TAG_BYTES];
};

#define TELEX_LEAK_KEY

typedef struct TelexTunnel_st *TelexTunnel;

TelexTunnel telex_tunnel_new(void);
void telex_tunnel_free(TelexTunnel tun);

// Connect, send, and recv use the socket API convention of returning
// <= 0 on failure.


// Nonblocking connect API functions: prepare_connect, finish_connect,

// Synchronous function. Call before finish_connect.
// Returns 0 on success.
int telex_tunnel_prepare_connect(TelexTunnel tun, int sock, int nonblock);

// Returns 0 on success. If it returns SSL_ERROR_WANT_READ or
// SSL_ERROR_WANT_WRITE (both > 0), you need to wait for the desired
// condition and call again. Returns <0 on fatal error.
int telex_tunnel_finish_connect(TelexTunnel tun, int nonblock);


// Synchronous connect from start to finish.
int telex_tunnel_connect(TelexTunnel tun, char *notblocked_host,
                                 int notblocked_port);
// Returns > 0 on success.
int telex_tunnel_send(TelexTunnel tun, const void *buf, int len);

// Returns the number of characters received, -1 on failure.
int telex_tunnel_recv(TelexTunnel tun, void *buf, int len);

// Returns >0 if the tunnel has data ready for reading.
// The return value may or may not be the number of bytes ready.
int telex_tunnel_peek(TelexTunnel tun);

void telex_tunnel_close(TelexTunnel tun);
#endif /* LIBTELEXSOCK_H */

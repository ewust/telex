#include "libtelextunnel.h"

#include <openssl/ssl.h>

#include "client.h"
#include "common.h"
#include "../util/telex_util.h"
#include "ssl_api.h"

#include <assert.h>


byte privkey[PTWIST_BYTES];

struct TelexTunnel_st * telex_tunnel_new(void) {
  static int initialized = 0;
  if (!initialized) {
    tag_init();
    tag_load_pubkey();
#ifdef CHECK_TAGS
    tag_load_privkey(privkey);
#endif
    initialized = 1;
  }

  struct TelexTunnel_st *ret = malloc(sizeof (struct TelexTunnel_st));
  if (!ret) {
    fprintf(stderr, "malloc failed\n");
    exit(-1);
  }
  ret->ssl = NULL;
  ret->sock = -1;
  return ret;
}

void telex_tunnel_free(struct TelexTunnel_st *victim) {
  free(victim);
}

int telex_tunnel_prepare_connect(TelexTunnel tun, int sock, int nonblock) {
  byte *tag = tun->tag;
  byte *shared_secret = tun->shared_secret;

  static SSL_CTX *ctx = 0;
  if (!ctx) {
    ctx = initialize_ctx(KEYFILE, PASSWORD);
  }
  assert(tun);

  tun->sock = sock;
  SSL *ssl = SSL_new(ctx);
  BIO *sbio = BIO_new_socket(tun->sock, BIO_NOCLOSE);
  if (nonblock) {
    BIO_set_nbio(sbio, 1);
  }
  SSL_set_bio(ssl, sbio, sbio);

  // Generate tag context string (server_ip || timestamp || session_id (=0x00 for now) )
  unsigned long t = htonl(time(NULL)); // client_random and context must share a timestamp
  unsigned long server_ip;
  get_remote_addr(tun->sock, &server_ip); 
  char *session_id = "\x00";

  unsigned char tag_context[MAX_CONTEXT_LEN]; 
  memcpy(&tag_context[0], &server_ip, 4);
  memcpy(&tag_context[4], &t, 4);
  memcpy(&tag_context[8], session_id, 1); 

#ifdef CHECK_TAGS
  byte dummy_key[16] = {0};
  do {
#endif
     
    gen_tag(tag, shared_secret, tag_context, MAX_CONTEXT_LEN);

#ifdef CHECK_TAGS
    check_tag(dummy_key, privkey, tag, tag_context, MAX_CONTEXT_LEN); 
  } while (memcmp(dummy_key, shared_secret, 16) != 0);
#endif

  ssl->telex_client_random = malloc(32); // TODO: find appropriate #defines
  memcpy(ssl->telex_client_random, &t, 4);
  memcpy(ssl->telex_client_random+4, tag, 28);

  ssl->telex_dh_priv_key = NULL;
#ifdef TELEX_LEAK_KEY
  ssl->telex_dh_priv_key = telex_ssl_get_dh_key(shared_secret, NULL); 
#endif
  tun->ssl = ssl;

/*
  char dummy_key[16];

  check_tag(dummy_key, privkey, tag, "context", strlen("context")); 

  if (memcmp(dummy_key, shared_secret, 16) != 0) {
    printf("complain: mismatched keys\n");
  }
*/
  if (!nonblock) {
    return telex_tunnel_finish_connect(tun, nonblock);
  } else {
    return 0;
  }
}

static int telex_tunnel_after_connect(TelexTunnel tun, int nonblock)
{

#ifndef TELEX_LEAK_KEY
  byte *shared_secret = tun->shared_secret;
  BIO *sbio;

  if (tun->ssl) {
    SSL_free(tun->ssl);
  }
  tun->ssl = get_live_ssl_obj(shared_secret, 16, 0);
  printf("TELEX_LEAK_KEY undefined!\n");
  // Need to reset the BIO because we've got a new SSL object here.
  sbio = BIO_new_socket(tun->sock, BIO_NOCLOSE);
  if (nonblock) {
    BIO_set_nbio(sbio, 1);
  }
  SSL_set_bio(tun->ssl, sbio, sbio);
  if (!tun->ssl) {
    fprintf(stderr, "Couldn't switch to telex crypto...\n");
    return -1;
  }
#endif

#if 0
  printf("\nread_mac_secret:\n");
  hexdump(tun->ssl->s3->read_mac_secret, 20);
  printf("\nwrite_mac_secret:\n");
  hexdump(tun->ssl->s3->write_mac_secret, 20); 
    printf("tun->ssl->enc_write_ctx: %p\n", tun->ssl->enc_write_ctx);
  printf("\nwrite_ctx_oiv:\n");
  hexdump(tun->ssl->enc_write_ctx->oiv, 16);
  printf("\nwrite_ctx_iv:\n");
  hexdump(tun->ssl->enc_write_ctx->iv, 16);
  printf("\nread_ctx_oiv:\n");
  hexdump(tun->ssl->enc_read_ctx->oiv, 16);
  printf("\nread_ctx_iv:\n");
  hexdump(tun->ssl->enc_read_ctx->iv, 16);
  printf("\n");
#endif

  return 0;
}

int telex_tunnel_finish_connect(TelexTunnel tun, int nonblock) {
  int ret = SSL_connect(tun->ssl);
  int err = SSL_get_error(tun->ssl, ret);
  switch (err) {
  case SSL_ERROR_NONE:
    return telex_tunnel_after_connect(tun, nonblock);
  case SSL_ERROR_WANT_READ:
  case SSL_ERROR_WANT_WRITE:
    if (nonblock) {
      return err;
    } else {
      fprintf(stderr, "SSL connect error %d\n", err);
      return -1;
    }
  default:
    fprintf(stderr, "SSL connect error %d\n", err);
    return -1;
  }
}


int telex_tunnel_connect(TelexTunnel tun, char *notblocked_host,
                         int notblocked_port) {
  telex_tunnel_prepare_connect(tun,
                               tcp_connect(notblocked_host,
                                           notblocked_port),
                               0);
  return telex_tunnel_finish_connect(tun, 0);
}

int telex_tunnel_send(TelexTunnel tun, const void *buf, int len) {
  assert(tun);
  return SSL_write(tun->ssl, buf, len);
}

int telex_tunnel_recv(TelexTunnel tun, void *buf, int len) {
  assert(tun);
  return SSL_read(tun->ssl, buf, len);
}

int telex_tunnel_peek(TelexTunnel tun) {
  return SSL_pending(tun->ssl);
}

void telex_tunnel_close(TelexTunnel tun) {
  if (tun->ssl) {
    SSL_shutdown(tun->ssl);
    SSL_free(tun->ssl);
    tun->ssl = 0;
  }
  close(tun->sock);
  tun->sock = -1;
}

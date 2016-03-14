#ifndef SSL_API_H
#define SSL_API_H


#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <stdlib.h>

#include <openssl/ssl.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <string.h>

//#include "ssl/ssl_locl.h"
int ssl3_new(SSL *s);
int ssl3_connect(SSL *s);
EVP_MD_CTX *ssl_replace_hash(EVP_MD_CTX **hash,const EVP_MD *md);
int tls1_change_cipher_state(SSL *s, int which);
int ssl_get_new_session(SSL *s, int session);
const SSL_CIPHER *ssl3_get_cipher_by_char(const unsigned char *p);
int tls1_generate_master_secret(SSL *s, unsigned char *out, unsigned char *p,
                                int len);
int tls1_setup_key_block(SSL *s);

struct secrets {
    char    key[32];
    char    iv[16];
    char    mac_secret[20];
};
#define TELEX_LEAK_KEY

int my_ssl_write(BIO *b, const char *in, int len);

int my_ssl_read(BIO *b, char *out, int len);

SSL* get_live_ssl_obj(void *telex_secret, unsigned int telex_secret_length, void *client_random, void *server_random, int is_server);

/* Given a "live" SSL object, change its secrets for communication via Telex. */
int switch_to_telex_crypto(SSL *ssl, void *telex_secret, unsigned int telex_secret_length, int is_server);

int ssl_encrypt(SSL *s, const char *in, int len, char **out);
int ssl_shutdown(SSL *s, char **out);

int ssl_decrypt(SSL *s, const char *in, int len, char **out);
BIGNUM *telex_ssl_get_dh_key(void *telex_secret, BIGNUM *r);
int telex_get_shared_secret(BIGNUM *client_dh_priv_key, BIGNUM *p, BIGNUM *server_dh_pub_key, char *shared_secret);

#ifdef __cplusplus
}
#endif


#endif /* SSL_API_H */

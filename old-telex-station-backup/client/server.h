#ifndef _server_h
#define _server_h

#define KEYFILE "server.pem"
#define PASSWORD "password"
#define DHFILE "dh1024.pem"

int tcp_listen(int port);
void load_dh_params(SSL_CTX *ctx,char *file);
void generate_eph_rsa_key(SSL_CTX *ctx);

#endif


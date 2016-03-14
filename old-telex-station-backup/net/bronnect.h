#ifndef BRONNECT_H
#define BRONNECT_H


#include "telex_tcp.h"      //for the struct flow and friends

#include <openssl/ssl.h>
#include <openssl/err.h>  // For ERR_print_errors.
#include "client.h"
#include "ssl_api.h"
#include "../util/telex_util.h"


#define RECORD_TYPE_INIT                0x0000
#define RECORD_TYPE_ENCRYPTED_DATA      0x0001
#define RECORD_TYPE_RAW_PKT             0x0002

#define TELEX_LEAK_KEY

struct ConnectionState {
  int cnum;
  int telex_conn;  // For communication with the Bro side.
  int service_conn;  // For communication with the Telex service.
  int client_conn;  // For communication with the Telex client.
  int first_telex_record;
  struct flow flow_state;
  struct event *ev_telex_read;
  struct event *ev_service_read;
  struct event *ev_service_write;
  struct event *ev_client_read;
  struct event *ev_client_write;
#ifdef WATCHDOG
  struct event *ev_watchdog;
#endif
  SSL *ssl;
  int client_ready[2];
  int service_ready[2];
  int cleanup_waiting;  // During cleanup_connection, could WANT_READ
                        // so we let on_read_ready fire again and do it there.
  int state;            // State for determining what needs to be cleaned up when
                        // watchdog expires:
                        //  0 - after accept (just initialized)
                        //  1 - after init data has been received (need to clean up SSL object
                        //      and fake ioctl socket).
                        //  2 - after raw packet has been forwarded (same as above) 
                        //  3 - we've logged the first line of this connection 
                        //  4 - cleanup_connection_ssl called
                        //  5 - cleanup_connection_try_ssl
  int key_port;         // port given by bro (depending on key), decides which port service_conn
                        // connects to
  time_t t_created;
  time_t t_last_data;
  time_t t_cleanup;
  time_t t_watchdog;
};

struct bro_record_hdr {
    uint16_t        type;
    uint16_t        len;        //ssl records have a max length of like...16KB. This is ok
} __attribute__((__packed__));


struct dh_params {
    uint16_t        p_len;
    unsigned char   p[128];

    uint16_t        g_len;
    unsigned char   g[128];

    uint16_t        pub_key_len;
    unsigned char   pub_key[128];

    unsigned char   client_random[32];
    unsigned char   server_random[32];
    unsigned char   client_iv[16];
    unsigned char   server_iv[16];
    uint8_t         compression_method;
} __attribute__((__packed__));


struct bro_init_data {
    struct flow         flow_state; //from telex_tcp.h

    //ssl
    unsigned char       shared_secret[16]; 
    uint16_t            port;
    struct dh_params    dh;
} __attribute__((__packed__)); 


void cleanup_connection(struct ConnectionState *st);
void cleanup_connection_ssl_try_shutdown(struct ConnectionState *st);
void cleanup_connection_after_ssl(struct ConnectionState *st, int unblock);
void on_service_read(int conn, short type, struct ConnectionState *st);
void on_finish_connect(int conn, short type, struct ConnectionState *st);
void log_data(struct ConnectionState *st, char *buf, int len);

#define READ 0
#define WRITE 1

#endif

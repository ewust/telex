#ifndef HTTPD_H
#define HTTPD_H

#include <glib.h>

#define MAX_PORTS 15
#define MAX_HTTPD_LINE_SIZE (125 + (MAX_PORTS * 20))

struct bronnect_stats {
    uint64_t    down_bytes;
    uint64_t    up_bytes;

    uint32_t    active_connections;
    uint32_t    total_connections;
};

struct port_stats {
    GHashTable *users;
    int         port;
    uint64_t    users_size;
    uint64_t    connections;
};

void httpd_init(int port);
void update_port_usage(GHashTable *port_users, int key_port, unsigned int client_ip);

extern struct bronnect_stats stats;

#endif

/* Copyright 2012, 2013, 2014  University of Michigan

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

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

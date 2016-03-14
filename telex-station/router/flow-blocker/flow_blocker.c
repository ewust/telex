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

#include "telex_block_flow.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <event.h>
#include <glib.h>

#define FLOW_TIMEOUT (30 * 60 * 60)  /* 30 minutes. */

struct flow {
  struct in_addr addr1;
  struct in_addr addr2;
  uint16_t port1;
  uint16_t port2;
  struct event *ev;
};

struct state {
  struct event ev_read;
};

static GQueue flow_timeout_queue = G_QUEUE_INIT;

static void unblock_flow(int fd, short event_type, struct flow *the_flow) {
  telex_local_mod_flow(&the_flow->addr1, the_flow->port1, &the_flow->addr2,
                       the_flow->port2, AF_INET, UNBLOCK);
  g_slice_free(struct event, the_flow->ev);
  g_slice_free(struct flow, the_flow);
};

void cleanup_connection(int fd, struct state *st) {
  event_del(&st->ev_read);
  free(st);
  close(fd);
}

void on_read_ready(int conn, short type, struct state *st) {
  int status;
  char msgbuf[BLOCK_FLOW_MSG_LENGTH];

  status = recv(conn, &msgbuf, BLOCK_FLOW_MSG_LENGTH, MSG_WAITALL);
  if (status < BLOCK_FLOW_MSG_LENGTH) {
    if (status <= 0) {
      perror("Couldn't receive message");
      cleanup_connection(conn, st);
    }
    return;
  }

  struct flow *the_flow = g_slice_new(struct flow);
  char *p = msgbuf;
  int block = *p++ ? 1 : 0;

  memcpy(&the_flow->addr1.s_addr, p, sizeof(the_flow->addr1.s_addr));
  p += sizeof(the_flow->addr1.s_addr);

  memcpy(&the_flow->port1, p, sizeof(the_flow->port1));
  p += sizeof(the_flow->port1);

  memcpy(&the_flow->addr2.s_addr, p, sizeof(the_flow->addr2.s_addr));
  p += sizeof(the_flow->addr2.s_addr);

  memcpy(&the_flow->port2, p, sizeof(the_flow->port2));
  p += sizeof(the_flow->port2);

  assert((p - msgbuf) == BLOCK_FLOW_MSG_LENGTH);
  telex_local_mod_flow(&the_flow->addr1, the_flow->port1, &the_flow->addr2, the_flow->port2, AF_INET, block);

  struct timeval timeout = {FLOW_TIMEOUT, 0};
  the_flow->ev = g_slice_new(struct event);
  timeout_set(the_flow->ev, (void(*)(int, short, void *))unblock_flow, the_flow);
  timeout_add(the_flow->ev, &timeout);
}

void on_accept_ready(int sock, short type, void *arg) {
  int conn;
  if ((conn = accept(sock, NULL, NULL)) == -1) {
    perror("Couldn't accept");
    return;
  }

  struct state *st = malloc(sizeof(*st));
  event_set(&st->ev_read, conn, EV_READ | EV_PERSIST,
            (void(*)(int, short, void*))on_read_ready, st);
  event_add(&st->ev_read, NULL);
}

static int sock;

static void close_listen_sock(void) {
  fprintf(stderr, "closing\n");
  close(sock);
}

static void handle_sigint(int n) {
  exit(0);
}

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s ip port\n", argv[0]);
    return 1;
  }

  char *listen_ip = argv[1];
  int listen_port = atoi(argv[2]);
  struct in_addr ip_addr;

  if (!inet_aton(listen_ip, &ip_addr)) {
    fprintf(stderr, "Bogus IP %s\n", listen_ip);
    return 1;
  }

  if (listen_port == 0 || listen_port >= 65536) {
    fprintf(stderr, "Invalid port number %d\n", listen_port);
    return 1;
  }

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Couldn't create socket");
    return 2;
  }

  struct sockaddr_in addr;
  addr.sin_addr = ip_addr;
  addr.sin_port = htons(listen_port);
  addr.sin_family = AF_INET;
  if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("Couldn't bind");
    return 2;
  }

  if (listen(sock, 5) == -1) {
    perror("Couldn't listen");
    return 2;
  }

  atexit(close_listen_sock);
  struct sigaction act;
  act.sa_handler = handle_sigint;
  sigaction(SIGINT, &act, NULL);
  sigaction(SIGTERM, &act, NULL);
  telex_block_flow_init();
  event_init();
  struct event accept_event;
  event_set(&accept_event, sock, EV_READ | EV_PERSIST,
            (void(*)(int, short, void*)) on_accept_ready, NULL);
  event_add(&accept_event, NULL);
  event_dispatch();
  return 0;
}

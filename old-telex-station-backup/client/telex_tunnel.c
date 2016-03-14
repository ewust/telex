#include "libtelextunnel.h"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "server.h"
#include "common.h"
#include "../util/telex_util.h"
#include <event.h>

#include <assert.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/fcntl.h>

#include <glib.h>

#define READ 0
#define WRITE 1

struct state {
  TelexTunnel tun;
  int client_fd;
  struct event *ev_tun_read, *ev_tun_write;
  struct event *ev_client_read, *ev_client_write;
  struct event *finish_connection_event;
#ifdef WATCHDOG
  struct event *ev_watchdog;
#endif
  int cleanup_waiting;
  int tunnel_ready[2];
  int client_ready[2];
};

static void cleanup_connection_ssl_try_shutdown(struct state *st);
static void cleanup_connection(struct state *st);
static void finish_tunnel_connect(int conn, short type, struct state *st);
static void after_tunnel_connect(struct state *st);

#define WATCHDOG_TIMEOUT 15

#ifdef WATCHDOG
static void on_watchdog(int conn, short type, struct state *st) {
  fprintf(stderr, "Watchdog expired, src port %d\n", get_port(st->tun->sock));
  cleanup_connection(st);
}
#endif

static void pet_watchdog(struct state *st) {
  #ifdef WATCHDOG
  if (st->ev_watchdog) {
    struct timeval timeout = {WATCHDOG_TIMEOUT, 0};
    event_del(st->ev_watchdog);
    timeout_add(st->ev_watchdog, &timeout);
  }
  #endif
}

static void initialize_watchdog(struct state *st) {
  #ifdef WATCHDOG
  struct timeval timeout = {WATCHDOG_TIMEOUT, 0};
  st->ev_watchdog = g_slice_new(struct event);
  timeout_set(st->ev_watchdog, (void(*)(int, short, void*))on_watchdog,
              st);
  timeout_add(st->ev_watchdog, &timeout);
  #endif
}


static void try_to_forward(struct state *st) {
  // Performance of this may have to be fixed later.
  if (st->tunnel_ready[READ] && st->client_ready[WRITE]) {
    char buf[4096];
    int nread = telex_tunnel_recv(st->tun, buf, sizeof(buf));
    if (nread > 0) {
      if (sendall(st->client_fd, buf, nread) < 0) {
        fprintf(stderr, "Error from send when forwarding\n");
        cleanup_connection(st);
        return;
      }
    } else if (nread < 0) {
      int err = SSL_get_error(st->tun->ssl, nread);
      switch (err) {
      case SSL_ERROR_ZERO_RETURN:
      case SSL_ERROR_WANT_CONNECT:
      case SSL_ERROR_WANT_ACCEPT:
      case SSL_ERROR_WANT_X509_LOOKUP:
      case SSL_ERROR_SYSCALL:
      case SSL_ERROR_SSL:
        fprintf(stderr, "SSL error %d reading from tunnel, src port: %d\n",
                err, get_port(st->tun->sock));
        if (err == SSL_ERROR_SSL) {
          extern BIO *bio_err;
          if (!bio_err) {
            bio_err = BIO_new_fp(stderr, BIO_NOCLOSE);
          }
          ERR_print_errors(bio_err);
        } else if (err == SSL_ERROR_SYSCALL) {
          perror("SSL_ERROR_SYSCALL");
        }
        cleanup_connection(st);
        return;
      case SSL_ERROR_WANT_READ:
      case SSL_ERROR_WANT_WRITE:
        //fprintf(stderr, "Recoverable SSL error %d reading from tunnnel\n",
        //        err);
        goto RESET_EVENTS;  // Can't break, we're in a switch.
      default:
        fprintf(stderr, "Unexpected SSL error %d reading from tunnel\n",
                err);
        assert(0);
      } // end switch.
    } else {
      cleanup_connection(st);
      return;
    }
  RESET_EVENTS:
    st->tunnel_ready[READ] = st->client_ready[WRITE] = 0;
    event_add(st->ev_client_write, NULL);
    event_add(st->ev_tun_read, NULL);
  }

  if (st->client_ready[READ] && st->tunnel_ready[WRITE]) {
    char buf[4096];
    int nread = recv(st->client_fd, buf, sizeof(buf), 0);
    if (nread > 0) {
      int nsent = telex_tunnel_send(st->tun, buf, nread);
      if (nsent <= 0) {
        int err = SSL_get_error(st->tun->ssl, nsent);
        switch (err) {
        case SSL_ERROR_ZERO_RETURN:
        case SSL_ERROR_WANT_CONNECT:
        case SSL_ERROR_WANT_ACCEPT:
        case SSL_ERROR_WANT_X509_LOOKUP:
        case SSL_ERROR_SYSCALL:
        case SSL_ERROR_SSL:
          fprintf(stderr, "SSL error %d forwarding to telex, src port: %d\n",
                  err, get_port(st->tun->sock));
          if (err == SSL_ERROR_SSL) {
            extern BIO *bio_err;
            if (!bio_err)
              bio_err=BIO_new_fp(stderr,BIO_NOCLOSE);
            ERR_print_errors(bio_err);
          } else if (err == SSL_ERROR_SYSCALL) {
            perror("SSL_ERROR_SYSCALL");
          }
          cleanup_connection(st);
          return;
        case SSL_ERROR_WANT_READ:
        case SSL_ERROR_WANT_WRITE:
          //fprintf(stderr, "Recoverable SSL error %d forwarding to telex\n",
          //        err);
          break;
        default:
          assert(0);
        }
      }
    } else if (nread == 0) {
      cleanup_connection(st);
      return;
    } else {
      perror("Error recv(2)ing from client\n");
      fflush(stderr);
      cleanup_connection(st);
      return;
    }
    st->client_ready[READ] = st->tunnel_ready[WRITE] = 0;
    event_add(st->ev_client_read, NULL);
    event_add(st->ev_tun_write, NULL);
  }
}

static void on_read_ready(int fd, short event_type, struct state *st) {
  assert(event_type == EV_READ);
  if (st->client_fd == fd) {
    st->client_ready[READ] = 1;
    event_del(st->ev_client_read);
  } else {
    if (st->cleanup_waiting == SSL_ERROR_WANT_READ) {
      cleanup_connection_ssl_try_shutdown(st);
      return;
    }
    st->tunnel_ready[READ] = 1;
    event_del(st->ev_tun_read);
  }

  try_to_forward(st);
  pet_watchdog(st);
}

static void on_write_ready(int fd, short event_type, struct state *st) {
  assert(event_type == EV_WRITE);
  if (st->client_fd == fd) {
    st->client_ready[WRITE] = 1;
    event_del(st->ev_client_write);
  } else {
    if (st->cleanup_waiting == SSL_ERROR_WANT_WRITE) {
      cleanup_connection_ssl_try_shutdown(st);
      return;
    }

    st->tunnel_ready[WRITE] = 1;
    event_del(st->ev_tun_write);
  }

  try_to_forward(st);
  pet_watchdog(st);
}

char *notblocked_ip;
int notblocked_port;

static void cleanup_connection_after_ssl(struct state *st) {
  if (st->ev_tun_read) {
    if (!st->tunnel_ready[READ]) {
      event_del(st->ev_tun_read);
    }
    g_slice_free(struct event, st->ev_tun_read);
  }
  if (st->ev_tun_write) {
    if (!st->tunnel_ready[WRITE]) {
      event_del(st->ev_tun_write);
    }
    g_slice_free(struct event, st->ev_tun_write);
  }

#ifdef WATCHDOG
  if (st->ev_watchdog) {
    event_del(st->ev_watchdog);
    g_slice_free(struct event, st->ev_watchdog);
    st->ev_watchdog = NULL;
  }
#endif
  telex_tunnel_close(st->tun);
  telex_tunnel_free(st->tun);
  close(st->client_fd);
  free(st);
}

static void cleanup_connection_ssl_try_shutdown(struct state *st) {
  int r = SSL_shutdown(st->tun->ssl);
  if (r < 0) {
    r = SSL_get_error(st->tun->ssl, r);
    st->cleanup_waiting = r;
    switch (r) {
    case SSL_ERROR_NONE:
      break;
    case SSL_ERROR_WANT_READ:
    case SSL_ERROR_WANT_WRITE:
      return;
    default:
      fprintf(stderr, "SSL error %d trying shutdown of tunnel\n", r);
      break;
    }
  }
  cleanup_connection_after_ssl(st);
}

static void free_connection(struct state *st) {
  if (st->finish_connection_event) {
    event_del(st->finish_connection_event);
    g_slice_free(struct event, st->finish_connection_event);
  }
  if (st->ev_client_read) {
    if (!st->client_ready[READ]) {
      event_del(st->ev_client_read);
    }
    g_slice_free(struct event, st->ev_client_read);
  }
  if (st->ev_client_write) {
    if (!st->client_ready[WRITE]) {
      event_del(st->ev_client_write);
    }
    g_slice_free(struct event, st->ev_client_write);
  }

  // Don't call SSL_shutdown on NULL.
  int r = st->tun->ssl ? SSL_shutdown(st->tun->ssl) : 1;

  if (!r) {
    cleanup_connection_ssl_try_shutdown(st);
  } else {
    cleanup_connection_after_ssl(st);
  }
}

static GHashTable *connections;

static void cleanup_connection(struct state *st) {
  gboolean was_in_there = g_hash_table_remove(connections, st);
  assert(was_in_there);
  // We don't need to call free_connection, the hash table will call it for us.
}


static void close_all_connections(void) {
  g_hash_table_destroy(connections);
}

static void finish_handling_connection(int conn, short type,
                                       struct state *st) {
  int optval;
  socklen_t optlen = sizeof(optval);
  TelexTunnel the_tunnel = st->tun;

  g_slice_free(struct event, st->finish_connection_event);
  st->finish_connection_event = NULL;

  if (getsockopt(conn, SOL_SOCKET, SO_ERROR, &optval, &optlen) != 0) {
    perror("finish_handling_connection getsockopt");
    exit(1);
  }
  if (type & EV_TIMEOUT) {
    fprintf(stderr, "Timeout connecting to notblocked (port %d)\n",
            get_port(conn));
    cleanup_connection(st);
    return;
  }
  if (optval != 0) {
    fprintf(stderr, "Error connecting to notblocked (port %d): %s\n", 
            get_port(conn), strerror(optval));
    cleanup_connection(st);
    return;
  }

  int res = telex_tunnel_prepare_connect(the_tunnel, the_tunnel->sock, 1);
  if (res) {
    fprintf(stderr, "Fatal connection error %d from prepare\n", res);
    cleanup_connection(st);
    return;
  }

  pet_watchdog(st);
  finish_tunnel_connect(the_tunnel->sock, EV_READ, st);
}

static void finish_tunnel_connect(int conn, short type, struct state *st) {
  int res = telex_tunnel_finish_connect(st->tun, 1);
  if (!res) {
    after_tunnel_connect(st);
    return;
  }
  if (res < 0) {
    fprintf(stderr, "Fatal connection error %d from finish_connect\n", res);
    cleanup_connection(st);
    return;
  }

  assert(res == SSL_ERROR_WANT_READ || res == SSL_ERROR_WANT_WRITE);
  type = (res == SSL_ERROR_WANT_READ) ? EV_READ : EV_WRITE;

  if (!st->finish_connection_event) {
    st->finish_connection_event = g_slice_new(struct event);
  }
  event_set(st->finish_connection_event, st->tun->sock, type,
            (void (*)(int, short, void*))finish_tunnel_connect, st);
  event_add(st->finish_connection_event, NULL);
  pet_watchdog(st);
}

static void after_tunnel_connect(struct state *st) {

  g_slice_free(struct event, st->finish_connection_event);
  st->finish_connection_event = NULL;

  TelexTunnel the_tunnel = st->tun;
  st->tunnel_ready[READ] = st->tunnel_ready[WRITE] = 0;
  st->client_ready[READ] = st->client_ready[WRITE] = 0;

  st->ev_tun_read = g_slice_new(struct event);
  event_set(st->ev_tun_read, the_tunnel->sock, EV_READ | EV_PERSIST,
            (void(*)(int, short, void *))on_read_ready, st);
  event_add(st->ev_tun_read, NULL);

  st->ev_tun_write = g_slice_new(struct event);
  event_set(st->ev_tun_write, the_tunnel->sock, EV_WRITE | EV_PERSIST,
            (void(*)(int, short, void*))on_write_ready, st);
  event_add(st->ev_tun_write, NULL);

  st->ev_client_read = g_slice_new(struct event);
  event_set(st->ev_client_read, st->client_fd, EV_READ | EV_PERSIST,
            (void(*)(int, short, void*))on_read_ready, st);
  event_add(st->ev_client_read, NULL);

  st->ev_client_write = g_slice_new(struct event);
  event_set(st->ev_client_write, st->client_fd, EV_WRITE | EV_PERSIST,
            (void(*)(int, short, void*))on_write_ready, st);
  event_add(st->ev_client_write, NULL);
}

void handle_connection(int conn) {
    TelexTunnel the_tunnel = telex_tunnel_new();
    struct state *st = malloc(sizeof(*st));
    int need_finish = 0;

    memset(st, 0, sizeof(*st));
    g_hash_table_insert(connections, st, GINT_TO_POINTER(1));

    st->client_fd = conn;
    fcntl(conn, F_SETFL, O_NONBLOCK);
    the_tunnel->sock = start_tcp_connect(notblocked_ip, notblocked_port,
                                         &need_finish);
    st->tun = the_tunnel;
    if (the_tunnel->sock < 0) {
      fprintf(stderr, "Couldn't connect tunnel");
      telex_tunnel_free(the_tunnel);
      free(st);
      close(conn);
      return;
    } else if (!need_finish) {
      st->finish_connection_event = NULL;
      finish_handling_connection(conn, EV_WRITE, st);
    } else {
      struct timeval timeout = {WATCHDOG_TIMEOUT, 0};
      st->finish_connection_event = g_slice_new(struct event);
      event_set(st->finish_connection_event, the_tunnel->sock, EV_WRITE,
                (void(*)(int, short, void*))finish_handling_connection, st);
      event_add(st->finish_connection_event, &timeout);
    }
    initialize_watchdog(st);
}

void on_accept_ready(int sock, short type, void *arg) {
  int conn;
  if ((conn = accept(sock, NULL, NULL)) == -1) {
    perror("Couldn't accept");
    return;
  }

  handle_connection(conn);
}

int main(int argc, char **argv) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s listen_port unblocked_host unblocked_port",
            argv[0]);
    return 1;
  }

  int listen_port = atoi(argv[1]);
  int listen_sock = tcp_listen(listen_port);

  notblocked_ip = argv[2];
  notblocked_port = atoi(argv[3]);

  event_init();

  struct event accept_event;
  event_set(&accept_event, listen_sock, EV_READ | EV_PERSIST,
            (void(*)(int, short, void*)) on_accept_ready, NULL);
  event_add(&accept_event, NULL);
  connections = g_hash_table_new_full(NULL,
                                      NULL,
                                      (GDestroyNotify)free_connection,
                                      NULL);
  atexit(close_all_connections);
  event_dispatch();
  return 0;
}

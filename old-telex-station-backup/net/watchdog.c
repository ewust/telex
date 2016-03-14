#include "bronnect.h"
#include "watchdog.h"
#include <event.h>
#include <glib.h>

void on_watchdog(int conn, short type, struct ConnectionState *st) {
#ifdef WATCHDOG
  fprintf(stderr, "Watchdog expired for client %d in state %d\n", st->cnum, st->state);
  st->t_watchdog = time(NULL);
  if (st->state >= 1) { 
    cleanup_connection(st);     // Whole cleanup.
  } else if (st->state >= 0) {
    cleanup_connection_after_ssl(st, 0);   // No SSL shutdown.
    // Would like to cleanup, but Bro hasn't told us who we are cleaning up yet!
  }
#endif
}

void pet_watchdog(struct ConnectionState *st) {
#ifdef WATCHDOG
  if (st->ev_watchdog) {
    struct timeval timeout = {WATCHDOG_TIMEOUT, 0};
    event_del(st->ev_watchdog);
    timeout_add(st->ev_watchdog, &timeout);
  }
#endif
}

/* Can be called multiple times */
void initialize_watchdog(struct ConnectionState *st) {
#ifdef WATCHDOG
  struct timeval timeout = {WATCHDOG_TIMEOUT, 0};
  if (st->state == 0) {
    timeout.tv_sec = WATCHDOG_S0_TIMEOUT;
  }

  if (st->ev_watchdog) {
    event_del(st->ev_watchdog);
  } else {
    st->ev_watchdog = g_slice_new(struct event);
    timeout_set(st->ev_watchdog, (void(*)(int, short, void*))on_watchdog,
              st);
  }
  timeout_add(st->ev_watchdog, &timeout);
#endif
}



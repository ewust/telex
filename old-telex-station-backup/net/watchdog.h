#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "bronnect.h"
#include <event.h>

#ifdef WATCHDOG
#define WATCHDOG_TIMEOUT        3600  /* in seconds. */
#define WATCHDOG_S0_TIMEOUT     120 /* state 0 has shorter timeout */
#endif

void on_watchdog(int conn, short type, struct ConnectionState *st);
void pet_watchdog(struct ConnectionState *st);
void initialize_watchdog(struct ConnectionState *st);

#endif

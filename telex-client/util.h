#ifndef _UTIL_H_
#define _UTIL_H_

#include <event2/event.h>
#include <event2/util.h>
#include <stdio.h>

#ifdef __FreeBSD__
#include <arpa/inet.h>  
#include <netinet/in.h> 
#endif


#if !defined(LIBEVENT_VERSION_NUMBER) || LIBEVENT_VERSION_NUMBER < 0x02000100
#error "This version of Libevent is not supported; Get 2.0.1-alpha or later."
#endif

void address_to_string(struct sockaddr_in *sin, char *output, int outlen);
unsigned long long time_ms(void);

#endif//_UTIL_H_

#include <event2/util.h>

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#include "util.h"
#include "logger.h"

void address_to_string(struct sockaddr_in *sin, char *output, int outlen)
{
	assert(sin && sin->sin_family == AF_INET);

	const char *addr;
	char addrbuf[255];
	addr = evutil_inet_ntop(sin->sin_family, &sin->sin_addr, addrbuf, sizeof(addrbuf));

	snprintf(output, outlen, "%s:%d", addr, ntohs(sin->sin_port));
}

// Returns milliseconds in unix epoch
unsigned long long time_ms(void)
{
	struct timeval now;	
	gettimeofday(&now, NULL);	
	return now.tv_sec * 1000 + now.tv_usec/1000;
}

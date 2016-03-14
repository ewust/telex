
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/time.h>
#include "util.h"


uint32_t get_local_addr(int sock)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    getsockname(sock, (struct sockaddr *)&addr, &len);
    return addr.sin_addr.s_addr;
}

// Returns milliseconds in unix epoch
unsigned long long time_ms()
{
	struct timeval now;	
	gettimeofday(&now, NULL);
	return now.tv_sec * 1000 + now.tv_usec/1000;
}

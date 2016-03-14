#ifndef UTIL_H
#define UTIL_H

#include <sys/types.h> // For size_t.

#ifdef __cplusplus
extern "C" {
#endif

int sendall(int sock, const char *buf, size_t len);
void hexdump(void *data, int len);
int start_tcp_connect(char *host, int port, int *need_finish);

// May return < 0 on error; check errno.
int get_port(int sock);
int get_remote_addr(int sock, unsigned long *addr);
#ifdef __cplusplus
}
#endif
#endif /* UTIL_H */

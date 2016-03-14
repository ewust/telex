#include "telex_util.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>
#include <fcntl.h>

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sendall(int sock, const char *buf, size_t len) {
  const char *p = buf;
  while (len) {
    int sent = send(sock, p, len, 0);
    if (sent < 0) {
      return -1;
    }
    p += sent;
    len -= sent;
  }
  return 0;
}

void hexdump(void *in, int len)
{
    char *data = in;
    int i, j;
    for (i=0; i<len; i++) {
        if (i!=0 && ((i%16) == 0)) {
            printf("    ");
            for (j=0; j<16; j++) {
                unsigned char c = (unsigned char)data[i+j-16];
                if (c > 32 && c < 127) printf("%c", c);
                else printf(".");
            }
            printf("\n");
        }
        printf("%02X ", (unsigned char)data[i]);
    }

    if ((i%16) != 0) {
        for (j=(i%16); j<16; j++) {
            printf("   ");
        }
    }
    printf("    ");
    for (j=(i-(i%16)); j<i; j++) {
        unsigned char c = (unsigned char)data[j];
        if (c > 32 && c < 127) printf("%c", c);
        else printf(".");
    }
    printf("\n");
}

int start_tcp_connect(char *host, int port, int *need_finish) {
  struct hostent *hp;
  struct sockaddr_in addr = {0};
  int sock;
  int r;

  assert(need_finish);
  if (!(hp = gethostbyname(host))) {
    fprintf(stderr, "Couldn't resolve host\n");
    exit(1);
  }

  addr.sin_addr = *(struct in_addr*)hp->h_addr_list[0];
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Couldn't create socket");
    return sock;
  }
/*
  struct sockaddr_in sa_loc = {0};

  sa_loc.sin_family = AF_INET;
  sa_loc.sin_port = htons(12345);
  r = inet_pton(AF_INET, "10.3.0.2", &sa_loc.sin_addr.s_addr);
  printf("inet_pton returns %d\n", r);

  r = bind(sock, (struct sockaddr *)&sa_loc, sizeof(struct sockaddr));
  printf("bind returns %d\n", r);
*/
  fcntl(sock, F_SETFL, O_NONBLOCK);
  r = connect(sock, (struct sockaddr *)&addr,
              sizeof(addr));

  *need_finish = 0;
  if (r == 0) {
    return sock;
  } else if (errno == EINPROGRESS) {
    *need_finish = 1;
    return sock;
  } else {
    perror("start_tcp_connect");
    return r;
  }
}

int get_port(int sock) {
  struct sockaddr_in addr;
  socklen_t len = sizeof(addr);
  int r;
  if ((r = getsockname(sock, (struct sockaddr *)&addr, &len)) != 0) {
    return r;
  }

  return ntohs(addr.sin_port);
}

int get_remote_addr(int sock, unsigned long *out_addr) {
  struct sockaddr_in addr;
  socklen_t len = sizeof(addr);
  int r;
  if ((r = getpeername(sock, (struct sockaddr *)&addr, &len)) != 0) {
    return r;
  }

  memcpy(out_addr, &addr.sin_addr.s_addr, sizeof(addr.sin_addr.s_addr));
  return 0;
}


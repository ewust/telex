#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include <stdio.h>

#include <openssl/ssl.h>
#include "client.h"

int main(int argc, char **argv) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s host port nconns\n", argv[0]);
    return 1;
  }

  struct hostent *hp;
  struct sockaddr_in addr = {0};
  if (!(hp = gethostbyname(argv[1]))) {
    perror("Couldn't resolve host");
    return 2;
  }

  addr.sin_addr = *(struct in_addr*)hp->h_addr_list[0];
  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(argv[2]));
  int nconns = atoi(argv[3]);

  int i = 0;
  while (i++ < nconns) {
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
      perror("connect");
      return 3;
    }
    close(sock);
  }
  return 0;
}

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>

int main(int argc, char **argv) {
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

  int sock;
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

  if (listen(sock, 30) == -1) {
    perror("Couldn't listen");
    return 2;
  }

  for (;;) {
    int conn;
    if ((conn = accept(sock, NULL, NULL)) == -1) {
      perror("Couldn't accept");
    } else{
      fprintf(stderr, "closing\n");
      close(conn);
    }
  }

  return 0;
}

#include "libtelextunnel.h"

#include <openssl/ssl.h>
#include "server.h"


#include <stdio.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s unblocked_host unblocked_port",
            argv[0]);
    return 1;
  }

  TelexTunnel the_tunnel = telex_tunnel_new();
  if (telex_tunnel_connect(the_tunnel, argv[1], atoi(argv[2])) < 0) {
    fprintf(stderr, "Couldn't connect tunnel\n");
    return 3;
  }

  printf("going to telex_tunnel_send\n");
  
  char *msg = "hello!";
  if (telex_tunnel_send(the_tunnel, msg, strlen(msg)) < 0) {
    berr_exit("Error writing to tunnel!\n");
    return -1;
  }

  printf("going to recv\n");

  char recv_buf[4096];
  int nrecved = telex_tunnel_recv(the_tunnel, recv_buf, 4096);
  if (nrecved <= 0) {
    berr_exit("Error receiving from tunnel!\n");
    return -1;
  }

  printf("things are ok!\n");

  puts("Response:");
  fwrite(recv_buf, nrecved, 1, stdout);
  return 0;
}

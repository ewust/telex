#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <stdio.h>

int main(void) {
  int s = socket(AF_INET, SOCK_STREAM, 0);
  if (s == -1) {
    perror("socket");
    return 1;
  }

  int val; 
  if (ioctl(s, 0xFACE, &val) == -1) {
    perror("ioctl");
    return 2;
  }
  return 0;
}

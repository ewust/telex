#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>


int main() {
   
    struct sockaddr_in sa;
    int sock;
    char *msg = "message";

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        perror("socket");
        exit(-1);
    }

    memset(&sa, 0, sizeof(struct sockaddr_in));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(123);
    inet_aton("10.4.0.2", &sa.sin_addr); 

    while (1) {
        sleep(10);
        sendto(sock, msg, strlen(msg), 0, 
	       (struct sockaddr*)&sa, sizeof(struct sockaddr_in));
    }

    close(sock);

    return 0;
}

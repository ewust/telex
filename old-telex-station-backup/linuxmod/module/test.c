#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include "forge_socket.h"
#include <unistd.h>
#include <string.h>


void print_state(struct tcp_state *st)
{
    printf("\tsrcip: %x:%d\n\tdstip:%x:%d\n", \
           ntohl(st->src_ip), ntohs(st->sport), \
           ntohl(st->dst_ip), ntohs(st->dport));
    printf("\tseq: %x\n\tack:%x\n", st->seq, st->ack);
    printf("\tfamily: %x\n\tstate: %x\n\trefcnt: %x\n\tbound_dev_if: %d\n", \
           st->skc_family, st->skc_state, st->skc_refcnt, st->skc_bound_dev_if);
    printf("\tsnd_una: %x\n", st->snd_una);
    printf("\ttstamp_ok: %d\n", st->tstamp_ok);
    printf("\tsack_ok: %d\n", st->sack_ok);
    printf("\twscale_ok: %d\n", st->wscale_ok);
    printf("\tecn_ok: %d\n", st->ecn_ok);
    printf("\tsnd_wscale: %d\n", st->snd_wscale);
    printf("\trcv_wscale: %d\n", st->rcv_wscale);
    //Debug:
    printf("\tsnd_wnd: %x\n", st->snd_wnd);
    printf("\trcv_wnd: %x\n", st->rcv_wnd);
    printf("\tdefault_ca_ops: %d\n", st->icsk_ca_ops_default);
    printf("\ttcp_header_len: %d\n", st->tp_header_len);
    printf("\tcopied_seq: %x\n", st->tp_copied_seq);
    printf("\trcv_wup: %x\n", st->tp_rcv_wup);
    printf("\tsnd_sml: %x\n", st->tp_snd_sml);
    printf("\tca_name: %s\n", st->icsk_ca_name);
    printf("\tinet_num: %d\n", st->inet_num);
    printf("\thas_icsk_bind_hash: %d\n", st->has_icsk_bind_hash);
    
    
}

// Fills in all but the src/dst ip/port and seq/ack numbers
// with some sane defaults
void default_state(struct tcp_state *st)
{
    st->tstamp_ok = 0;
    st->sack_ok = 0;
    st->wscale_ok = 0;
    st->ecn_ok = 0;
    st->snd_wscale = 0;
    st->rcv_wscale = 0;
    st->snd_wnd = 0x1000;
    st->rcv_wnd = 0x1000; 
    //make sure you set snd_una = seq (TODO: fix this in module)
}


int set_sock_state(int sock, struct tcp_state *st)
{
    struct sockaddr_in sin;
    sin.sin_family      = AF_INET;
    sin.sin_addr.s_addr = st->src_ip;
    sin.sin_port        = st->sport;

/*
     // TOOD: do we need/want this?
    if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, iface, strlen(iface)) < 0) {
        perror("setsockopt SO_BINDTODEVICE");
        return -1;
    }
*/

    int value = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) < 0) {
        perror("setsockopt SO_REUSEADDR");
        return -1;
    }

    if (setsockopt(sock, SOL_IP, IP_TRANSPARENT, &value, sizeof(value)) < 0) {
        perror("setsockopt IP_TRANSPARENT");
        return -1;
    }

    if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("bind");
        return -1;
    }

    if (setsockopt(sock, IPPROTO_TCP, TCP_STATE, st, sizeof(struct tcp_state)) < 0) {
        perror("setsockopt TCP_STATE");
        return -1;
    }

    return 0;

}

/* 
* Tests setsockopt by creating two (local) sockets 
* with arbitrary corresponding states, and passing data
* between them
*/
int test_dual()
{

    int sock1_port = 5555;
    int sock2_port = 7777;
    struct tcp_state state1, state2;

    int sock1 = socket(AF_INET, SOCK_FORGE, 0);
    int sock2 = socket(AF_INET, SOCK_FORGE, 0);

    
    default_state(&state1);
    default_state(&state2);
    
    state1.src_ip = state2.dst_ip = inet_addr("127.0.0.1");
    state1.dst_ip = state2.src_ip = inet_addr("127.0.0.1");
    state1.sport  = state2.dport  = htons(sock1_port);
    state1.dport  = state2.sport  = htons(sock2_port);
    state1.seq    = state2.ack    = 0xabcd0123;
    state1.ack    = state2.seq    = 0xdeadbeef;

    // fix this in kernel mod
    state1.snd_una = state1.seq;
    state2.snd_una = state2.seq;

    if (set_sock_state(sock1, &state1) != 0) {
        printf("[-] sock1 set_sock state failed!\n");
        return 1;
    }
    
    if (set_sock_state(sock2, &state2) != 0) {
        printf("[-] sock2 set_sock state failed!\n");
        return 1;
    }

    int r;
    char *msg = "Hello, world!";
    r = send(sock1, msg, strlen(msg), 0);
    printf("[+] send returned %d\n", r);

    char msg2[100];
    r = recv(sock2, msg2, sizeof(msg2), 0);
    if (r < 0) {
        perror("[-] dual test recv");
        return 1;
    }
    msg2[r] = '\x00';
    printf("[+] received '%s'\n", msg2);
    
    msg = "Pong!";
    r = send(sock2, msg, strlen(msg), 0);
    printf("[+] send returned %d\n", r);
    
    r = recv(sock1, msg2, sizeof(msg2), 0);
    if (r < 0) {
        perror("[-] dual test recv");
        return 1;
    }
    msg2[r] = '\x00';
    printf("[+] received '%s'\n", msg2);

    if (close(sock1) != 0) {
        perror("[-] dual test close sock1");
        return 1;
    }
 
    if (close(sock2) != 0) {
        perror("[-] dual test close sock2");
        return 1;
    }

    return 0;
}


int test_perms()
{
    int sock = socket(AF_INET, SOCK_FORGE, 0);
    struct tcp_state state;
    int len;
    int r;
    memset(&state, 0, sizeof(state));
    r = getsockopt(sock, IPPROTO_TCP, TCP_STATE, &state, &len);

    if (r < 0) {
        perror("[+] getsockopt:");
        return 0;
    }

    return 1;
}

int main()
{
    

    //test_dual();
    return test_perms();


    int sock;
    struct sockaddr_in sin;

    sock = socket(AF_INET, SOCK_FORGE, 0);
    if (sock < 0) {
        perror("socket");
        return -1;
    }
    sin.sin_family      = AF_INET;
    sin.sin_addr.s_addr = inet_addr("141.212.109.163");
    sin.sin_port        = htons(443);

    if (connect(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("connect");
        return -1;
    }

    int r, len;
    struct tcp_state state, state2;
    r = getsockopt(sock, IPPROTO_TCP, TCP_STATE, &state, &len);
    printf("connected socket returned %d\n", r);
    print_state(&state);


    int sock2 = socket(AF_INET, SOCK_FORGE, 0);
    /*
    r = getsockopt(sock2, IPPROTO_TCP, TCP_STATE, &state2, &len);
    printf("new socket (%d)\n", r);
    print_state(&state2);
    */

    state.src_ip = inet_addr("4.2.2.1");
    state.sport = htons(1234);
    set_sock_state(sock2, &state);

    r = getsockopt(sock2, IPPROTO_TCP, TCP_STATE, &state2, &len);
    printf("set sock (%d)\n", r);
    print_state(&state2);

    // Oh boy...here we go
    char *msg = "Hello, world!\n";
    r = send(sock2, msg, strlen(msg), 0);
    printf("send returned %d\n", r);

    sleep(5);
    return 0;
}

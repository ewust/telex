#ifndef TELEX_TCP_H
#define TELEX_TCP_H

#ifdef __cplusplus
extern "C" {
#endif


#if defined(__linux__) && !defined(_BSD_SOURCE)
#define _BSD_SOURCE
#endif
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
//#include <netinet/ip.h> //broken
#include <netinet/tcp.h>
#include <sys/time.h>




//fine, if netinet/ip.h won't work,
//i can do this myself
struct iphdr
{
    unsigned int ihl:4;
    unsigned int version:4;

    u_int8_t tos;
    u_int16_t tot_len;
    u_int16_t id;
    u_int16_t frag_off;
    u_int8_t ttl;
    u_int8_t protocol;
    u_int16_t check;
    u_int32_t saddr;
    u_int32_t daddr;
};

#if 0
struct tcphdr;

struct tcphdr
  {
    u_int16_t source;
    u_int16_t dest;
    u_int32_t seq;
    u_int32_t ack_seq;

    u_int16_t res1:4;
    u_int16_t doff:4;
    u_int16_t fin:1;
    u_int16_t syn:1;
    u_int16_t rst:1;
    u_int16_t psh:1;
    u_int16_t ack:1;
    u_int16_t urg:1;
    u_int16_t res2:2;

    u_int16_t window;
    u_int16_t check;
    u_int16_t urg_ptr;
};
#endif


#define IP_TCP(ip_hdr)      (struct tcphdr*)(((char *)ip_hdr) + (4 * ip_hdr->ihl))
#define TCP_DATA(tcp_hdr)   (((char *)tcp_hdr) + (4 * tcp_hdr->th_off))




//packet format
struct tcp_option_ts {
    uint16_t    nops;       // 0x0101
    uint8_t     op_type;    // 0x08
    uint8_t     op_len;     // 0x0a
    uint32_t    tsval;
    int32_t    tsecr;
} __attribute__((__packed__));

struct telex_tcp_ts {
    struct timeval tv;
    uint32_t       tsval;
    uint32_t       tsecr;
} __attribute__((__packed__));

struct telex_tcp_info {
    //last seq/ack
    uint32_t        seq;
    uint32_t        ack;

    uint16_t        rcv_window;
    uint16_t        rcv_window_scale;

    uint16_t        snd_window;
    uint16_t        snd_window_scale;
    
    //tcp timestamp option (rfc1323)
    struct telex_tcp_ts last_ts;
    struct telex_tcp_ts cur_ts;
    uint8_t         option_ts;  //1 for use timestamps
    uint8_t         option_sack;
    uint8_t         flags;
} __attribute__((__packed__));

struct flow {
    uint32_t    orig_h;
    uint32_t    resp_h;
    
    uint16_t    orig_p;
    uint16_t    resp_p;

    uint8_t     last_ttl;
    uint16_t    last_ip_id;  
    struct telex_tcp_info tcp;
} __attribute__((__packed__));

void telex_tcp_init(void);
int telex_fwd_pkt(char *pkt, int len);
int telex_tcp_forge_xmit(struct flow *fl, char *payload, int len);

extern int raw_sock;


#ifdef __cplusplus
}
#endif
#endif


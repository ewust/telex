#ifndef _FORGE_H
#define _FORGE_H

#include <linux/types.h>

#define SOCK_FORGE      9   // new protocol 
#define TCP_STATE       18  // new TCP sockopt

//Debug:
#define TCP_CA_NAME_MAX 16

struct tcp_state {
    __be32      src_ip;
    __be32      dst_ip;

    __be16      sport;
    __be16      dport;
    __u32       seq;
    __u32       ack;
    __u32	    snd_una;	/* First byte we want an ack for. SET THIS! */

    __u8        tstamp_ok;  // Did SYN packet have timestamp?
    __u8        sack_ok;  // Did SYN packet have SACK? (heh heh)
    __u8        wscale_ok;  // Did SYN packet have Wscale?
    __u8        ecn_ok;  // DO YOU WANT ECN?
    __u8        snd_wscale;  // Set if wscale_ok.
    __u8        rcv_wscale;  // Ditto.
    
    __u32       snd_wnd;	/* The window we expect to receive SET THIS! */
    __u32       rcv_wnd;




    __u32       ts_recent;  // Timestamp to echo next.
    __u32       ts_val;   // Timestamp to use next.

    __u32       mss_clamp; /* XXX: I guess this is negotiated in the handshake? Eric? */


    // Extra testing/debugging:
    unsigned short  skc_family;
    unsigned char   skc_state;
    int             skc_refcnt;
    int             skc_bound_dev_if;

    int             icsk_ca_ops_default;
    __u16           tp_header_len;
    __u32           tp_copied_seq;
    __u32           tp_rcv_wup;
    __u32           tp_snd_sml;
    char            icsk_ca_name[TCP_CA_NAME_MAX];
    __u16           inet_num;
    int             has_icsk_bind_hash;

};


#endif

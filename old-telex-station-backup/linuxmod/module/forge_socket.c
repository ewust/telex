#include <linux/module.h>   /* Needed by all modules */
#include <linux/kernel.h>   /* Needed for KERN_INFO */
#include <net/protocol.h>
#include "tcp_sk_forge.h"
//#include <linux/atomic.h>
#include <linux/file.h>
#include <linux/jiffies.h>
#include <linux/syscalls.h>
#include <linux/types.h>
#include <linux/tcp.h>
#include <net/inet_connection_sock.h>
#include <net/tcp.h>
#include <net/tcp_states.h>
#include <net/inet_common.h>
#include <net/inet_timewait_sock.h>
#include <net/sock.h>
#include <net/ip.h>
#include <net/route.h>

#include <asm/uaccess.h>
/* sk should be the struct sock from our original struct socket passed to accept. */


#include "forge_socket.h"


// TODO: move these to kernel specific header file
int forge_setsockopt(struct sock *sk, int level, int optname, char __user *optval,
                     unsigned int optlen);
int forge_getsockopt(struct sock *sk, int level, int optname, char __user *optval,
                     int __user *optlen);

struct proto forge_prot = {
    .name                   = "FORGE",
    .owner                  = THIS_MODULE,
    .close                  = tcp_close,
    .connect                = tcp_v4_connect,
    .disconnect             = tcp_disconnect,
    .accept                 = inet_csk_accept,
    .ioctl                  = tcp_ioctl,
    .init                   = NULL, //tcp_v4_init_sock, (filled in in init)
    .destroy                = tcp_v4_destroy_sock,
    .shutdown               = tcp_shutdown,
    .setsockopt             = forge_setsockopt,
    .getsockopt             = forge_getsockopt,
    .recvmsg                = tcp_recvmsg,
    .backlog_rcv            = tcp_v4_do_rcv,
    //.hash                   = inet_hash,
    //.unhash                 = inet_unhash,
    //.get_port               = inet_csk_get_port,
    .enter_memory_pressure  = tcp_enter_memory_pressure,
    .sockets_allocated      = &tcp_sockets_allocated,
    //.orphan_count           = &tcp_orphan_count,
    .memory_allocated       = &tcp_memory_allocated,
    .memory_pressure        = &tcp_memory_pressure,
    .sysctl_mem             = sysctl_tcp_mem,
    .sysctl_wmem            = sysctl_tcp_wmem,
    .sysctl_rmem            = sysctl_tcp_rmem,
    .max_header             = MAX_TCP_HEADER,
    .obj_size               = sizeof(struct tcp_sock),
    .slab_flags             = SLAB_DESTROY_BY_RCU,
    //.twsk_prot              = &forge_timewait_sock_ops, // equiv to tcp_timewait_sock_ops
//    .rsk_prot               = &forge_request_sock_ops, // equiv to tcp_request_sock_ops,
    .h.hashinfo             = &tcp_hashinfo,
#ifdef CONFIG_COMPAT
    .compat_setsockopt      = compat_tcp_setsockopt,
    .compat_getsockopt      = compat_tcp_getsockopt,
#endif
};


static struct inet_protosw forge_sw = {
    .type       = SOCK_FORGE,
    .protocol   = IPPROTO_TCP, 
    .prot       = &forge_prot,
    .ops        = &inet_stream_ops,
    .no_check   = 0,
    .flags      = INET_PROTOSW_ICSK,
                  //INET_PROTOSW_PERMANENT |
                  //INET_PROTOSW_ICSK,
};

int __init forge_init(void)
{
    int rc = -EINVAL;
    printk(KERN_INFO "Loaded forge_socket\n");

    // These members were not exported from the kernel,
    // so we use this hack to grab them from the exported tcp_prot struct.
    forge_prot.init = tcp_prot.init;
    forge_prot.hash = tcp_prot.hash;
    forge_prot.unhash = tcp_prot.unhash;
    forge_prot.get_port = tcp_prot.get_port;
    forge_prot.orphan_count = tcp_prot.orphan_count;
    
    // proto_register will only alloc these if they are null,
    // no sense in allocing more space - we can just use TCP's.
    // (though it will alloc .slab even if non-null - we let it).
    forge_prot.twsk_prot = tcp_prot.twsk_prot;
    forge_prot.rsk_prot = tcp_prot.rsk_prot;


    rc = proto_register(&forge_prot, 1);
    if (rc) {
        printk(KERN_CRIT "forge_init: Cannot register protocol (already loaded?)\n");
        return rc;
    }
 
    //sock_register() was done by af_inet, 
    //  and registers a create handler for socket(PF_INET, ...) - we don't want this

    //inet_add_protocol(&forge_proto, SOCK_FORGE);
    // adds handlers for receiving IPPROTO_* packets - we don't want this

    // Let's us (as per forge_prot) deal with SOCK_FORGE sockets.
    inet_register_protosw(&forge_sw);

    return 0;
}

void __exit forge_exit(void)
{

    // Currently, we're pointing to tcp_prot's twsk_prot and rsk_prot
    // and a call to proto_unregister will free these if non-null.
    // (We did allocate our own slab though, so proto_unregister will free
    //  that for us)
    forge_prot.rsk_prot = NULL;
    forge_prot.twsk_prot = NULL;

    inet_unregister_protosw(&forge_sw);
    proto_unregister(&forge_prot);

    printk(KERN_INFO "forge_socket: unloaded\n");
}


int forge_getsockopt(struct sock *sk, int level, int optname, char __user *optval,
                     int __user *optlen)
{
    struct tcp_state ret;

    if (optname == TCP_STATE) {
        if (!capable(CAP_NET_RAW)) {
            return -EACCES;
        }

        ret.ack     = tcp_sk(sk)->rcv_nxt;
        ret.seq     = tcp_sk(sk)->snd_nxt;
        ret.src_ip  = inet_sk(sk)->inet_rcv_saddr; // or inet_saddr?
        ret.dst_ip  = inet_sk(sk)->inet_daddr;
        ret.sport   = inet_sk(sk)->inet_sport;
        ret.dport   = inet_sk(sk)->inet_dport;

        ret.snd_una = tcp_sk(sk)->snd_una;
        ret.snd_wnd = tcp_sk(sk)->snd_wnd;
        ret.rcv_wnd = tcp_sk(sk)->rcv_wnd;
        
        ret.tstamp_ok = tcp_sk(sk)->rx_opt.tstamp_ok;
        ret.ecn_ok    = ((tcp_sk(sk)->ecn_flags & TCP_ECN_OK) != 0);
        ret.sack_ok   = tcp_sk(sk)->rx_opt.sack_ok;
        ret.wscale_ok = tcp_sk(sk)->rx_opt.wscale_ok;
        ret.snd_wscale= tcp_sk(sk)->rx_opt.snd_wscale;
        ret.rcv_wscale= tcp_sk(sk)->rx_opt.rcv_wscale;
        

        // Debugging/testing:
        ret.skc_refcnt = sk->sk_refcnt.counter;
        ret.skc_family = sk->sk_family;
        ret.skc_state  = sk->sk_state;
        ret.skc_bound_dev_if = sk->sk_bound_dev_if;

        ret.tp_header_len = tcp_sk(sk)->tcp_header_len;        
        ret.tp_copied_seq = tcp_sk(sk)->copied_seq;
        ret.tp_rcv_wup = tcp_sk(sk)->rcv_wup;
        ret.tp_snd_sml = tcp_sk(sk)->rcv_wup;

        ret.icsk_ca_ops_default = (inet_csk(sk)->icsk_ca_ops == &tcp_init_congestion_ops);
        strncpy(ret.icsk_ca_name, inet_csk(sk)->icsk_ca_ops->name, TCP_CA_NAME_MAX);

        ret.inet_num = inet_sk(sk)->inet_num;
        ret.has_icsk_bind_hash = (inet_csk(sk)->icsk_bind_hash != NULL);


        if (put_user(sizeof(ret), optlen))
            return -EFAULT;
        if (copy_to_user(optval, &ret, sizeof(ret)))
            return -EFAULT; 
        return 0;
    }
    return tcp_getsockopt(sk, level, optname, optval, optlen);
}
EXPORT_SYMBOL(forge_getsockopt);

/*
int forge_init_sock(struct sock *sk)
{
    return tcp_prot.init(sk);

}
EXPORT_SYMBOL(forge_init_sock);
*/
// TODO: ip_route_output_flow IS exported - why can't this find it?
// Unlike the real inet_csk_route_req, we return the rtable.
struct rtable *fake_inet_csk_route_req(struct sock *sk, struct tcp_state *st) {
    struct rtable *rt;
    // We don't care about IP options...
    struct flowi fl = {.oif = sk->sk_bound_dev_if,
                     .mark = sk->sk_mark,
                     .nl_u = { .ip4_u =
                               { .daddr = st->dst_ip,
                                 .saddr = st->src_ip,
                                 .tos = 0 }}, // TOS? DGAF.
                     .proto = sk->sk_protocol,
                     .flags = inet_sk_flowi_flags(sk),
                     .uli_u = { .ports =
                                { .sport = st->sport,
                                  .dport = st->dport } } };
    struct net *net = sock_net(sk);
    if (ip_route_output_flow(net, &rt, &fl, sk, 0)) {
        IP_INC_STATS_BH(net, IPSTATS_MIB_OUTNOROUTES);
        return NULL;
    }
    return rt;
}
EXPORT_SYMBOL(fake_inet_csk_route_req);


int forge_setsockopt(struct sock *sk, int level, int optname, char __user *optval,
                     unsigned int optlen)
{
    if (optname == TCP_STATE) {
        // TODO: check UID/permissions

        struct tcp_state st;
        struct inet_connection_sock *icsk;
        //struct inet_sock *isk;
        struct tcp_sock *tp;
        struct dst_entry *dst;
        struct rtable *rt;

        if (!capable(CAP_NET_RAW)) {
            return -EACCES;
        }

        if (copy_from_user(&st, (struct tcp_state __user *)optval, sizeof(st))) {
            return -EFAULT;
        }

        // syn_recv:
        icsk = inet_csk(sk);
        //isk = inet_sk(sk);
        tp = tcp_sk(sk);

        if ((rt = fake_inet_csk_route_req(sk, &st)) == NULL) {
            return -EINVAL;
        }
        //dst = &rt->dst;
        dst = &rt->u.dst; 
        
        inet_sk(sk)->inet_daddr = st.dst_ip;
        inet_sk(sk)->inet_rcv_saddr = st.src_ip;
        inet_sk(sk)->inet_saddr = st.src_ip;
        inet_sk(sk)->inet_id = tp->write_seq ^ jiffies;
        // TODO(swolchok): do I have to kmalloc a struct ip_options?
        inet_sk(sk)->opt = NULL;
        inet_sk(sk)->mc_index = rt->rt_iif; // Check this...should be inet_iif(skb).
        inet_sk(sk)->mc_ttl = 1;  // We are not going to multicast.
    
        icsk->icsk_ext_hdr_len = 0;
  
        // This should all work fine for us.
        tcp_mtup_init(sk);
        tcp_sync_mss(sk, dst_mtu(dst));
        tp->advmss = dst_metric(dst, RTAX_ADVMSS);
        if (tp->rx_opt.user_mss && tp->rx_opt.user_mss < tp->advmss) {
            tp->advmss = tp->rx_opt.user_mss;
        }
  
        tcp_initialize_rcv_mss(sk);
 

        // inet_csk_forge:
        // TODO: worry about endianness
        inet_sk(sk)->inet_dport = st.dport;  
        inet_sk(sk)->inet_num = ntohs(st.sport);
        inet_sk(sk)->inet_sport = st.sport;
        sk->sk_write_space = sk_stream_write_space;

        inet_csk(sk)->icsk_retransmits = 0;
        inet_csk(sk)->icsk_backoff = 0;
        inet_csk(sk)->icsk_probes_out = 0;
        /* Deinitialize accept_queue to trap illegal accesses. */
        memset(&icsk->icsk_accept_queue, 0, sizeof(icsk->icsk_accept_queue));
    

        //tcp_create_openreq_child:
        tp->pred_flags = 0;
        tp->rcv_wup = tp->copied_seq = tp->rcv_nxt = st.ack;

        tp->snd_sml = tp->snd_nxt = tp->snd_up = st.seq; /* + tcp_s_data_size(oldtp) */

        tcp_prequeue_init(tp);  // TODO(swolchok): check this.

        tp->srtt = 0;
        tp->mdev = TCP_TIMEOUT_INIT;
        icsk->icsk_rto = TCP_TIMEOUT_INIT;

        tp->packets_out = 0;
        tp->retrans_out = 0;
        tp->sacked_out = 0;
        tp->fackets_out = 0;
        tp->snd_ssthresh = TCP_INFINITE_SSTHRESH;

        tp->snd_cwnd = 2;
        tp->snd_cwnd_cnt = 0;
        tp->bytes_acked = 0;

        tp->frto_counter = 0;
        tp->frto_highmark = 0;

        icsk->icsk_ca_ops = &tcp_init_congestion_ops;
  
        tcp_set_ca_state(sk, TCP_CA_Open);
        tcp_init_xmit_timers(sk);
        skb_queue_head_init(&tp->out_of_order_queue);
        tp->write_seq = tp->pushed_seq = tp->snd_nxt;
  
        tp->rx_opt.saw_tstamp = 0;

        tp->rx_opt.dsack = 0;
        tp->rx_opt.num_sacks = 0;

        tp->urg_data = 0;

        //if (sock_flag(sk, SOCK_KEEPOPEN))
        //    inet_csk_reset_keepalive_timer(sk, keepalive_time_when(tp));  // TODO: fill in

        tp->rx_opt.tstamp_ok = st.tstamp_ok ? 1 : 0;
        if ((tp->rx_opt.sack_ok = st.sack_ok) != 0) {
            //if (sysctl_tcp_fack) {     // TODO: fill in (needed?)
            //    tcp_enable_fack(tp);
            //}
        }

        // Just set it to the max. (TODO: fix)
        tp->window_clamp = 65535 << 14;
        tp->rcv_ssthresh = tp->rcv_wnd = st.rcv_wnd << st.rcv_wscale;
    
        tp->rx_opt.wscale_ok = st.wscale_ok;
        if (tp->rx_opt.wscale_ok) {
            tp->rx_opt.snd_wscale = st.snd_wscale;
            tp->rx_opt.rcv_wscale = st.rcv_wscale;
        } else {
            tp->rx_opt.snd_wscale = tp->rx_opt.rcv_wscale = 0;
            tp->window_clamp = min(tp->window_clamp, 65535U);
        }

        tp->snd_wnd = st.snd_wnd << tp->rx_opt.snd_wscale;
        tp->max_window = tp->snd_wnd;

        if (tp->rx_opt.tstamp_ok) {
            tp->rx_opt.ts_recent = st.ts_recent;
            tp->rx_opt.ts_recent_stamp = get_seconds();
            // We want get_seconds() + ts_offset == st->ts_val.
            //tp->rx_opt.ts_offset = st->ts_val - tcp_time_stamp;
            tp->rx_opt.rcv_tsval = st.ts_val;

            tp->tcp_header_len = sizeof(struct tcphdr) + TCPOLEN_TSTAMP_ALIGNED;
        } else {
            tp->rx_opt.ts_recent_stamp = 0;
            tp->tcp_header_len = sizeof(struct tcphdr);
            //tp->rx_opt.ts_offset = 0;
            tp->rx_opt.rcv_tsval = 0;
        }

        // XXX: I don't know what this is...
#if 0
        if (skb->len >= TCP_MSS_DEFAULT + newtp->tcp_header_len)
            newicsk->icsk_ack.last_seg_size = skb->len - newtp->tcp_header_len;
#endif
        tp->rx_opt.mss_clamp = st.mss_clamp;
        tp->ecn_flags = st.ecn_ok ? TCP_ECN_OK : 0;
 

        sk->sk_socket->state = SS_CONNECTED;

        // recv_ack:
        tp->copied_seq = tp->rcv_nxt;
        smp_mb();  // Why not?
        tcp_set_state(sk, TCP_ESTABLISHED);  // We have to fake this.
        sk->sk_state_change(sk);

        tp->snd_una = tp->snd_up = st.snd_una;
        tcp_init_wl(tp, st.ack);

        // TODO(swolchok): use a real RTT measurement.
        //tcp_valid_rtt_meas(sk, msecs_to_jiffies(10)); // TODO: fill in
        //tcp_ack_update_rtt(sk, 0, 0);

        if (tp->rx_opt.tstamp_ok)
            tp->advmss -= TCPOLEN_TSTAMP_ALIGNED;

        icsk->icsk_af_ops->rebuild_header(sk);

        //tcp_init_metrics(sk);           // TODO: fill in

        //tcp_init_congestion_control(sk);  // TODO: fill in
        if (icsk->icsk_ca_ops->init)
            icsk->icsk_ca_ops->init(sk);

        tp->lsndtime = tcp_time_stamp;

        tcp_mtup_init(sk);
        tcp_initialize_rcv_mss(sk);
        //tcp_init_buffer_space(sk);    // TODO: fill in
        tcp_fast_path_on(tp);
        
        // If user did not call bind on this socket, we'll have to do this:
        //inet_csk_get_port(sk, st->sport); 
    
        __inet_hash_nolisten(sk, NULL);

        return 0;
    }
    return tcp_setsockopt(sk, level, optname, optval, optlen);
}
EXPORT_SYMBOL(forge_setsockopt);


module_init(forge_init);
module_exit(forge_exit);

// Originally adapted from Scott Wolchok's kernel patch.
MODULE_AUTHOR("Eric Wustrow");
MODULE_DESCRIPTION("Creates TCP sockets with arbitrary TCP/IP state (src, dst) (ports, seq, ack etc)");

MODULE_LICENSE("GPL");   // Will this let me call ip_route_output_flow??



#if 0 
int fake_accept_ioctl(struct sock *sk, unsigned long arg);


struct sock *inet_csk_forge(struct sock *sk, __be16 sport, __be16 dport,  const gfp_t priority) {
    struct sock *newsk = sk_clone(sk, priority);
    struct inet_connection_sock *newicsk = inet_csk(newsk);

    if (newsk == NULL) {
        return newsk;
    }

    newsk->sk_state = TCP_SYN_RECV;
#ifdef LINUX_PRE_2_6_32
    inet_sk(newsk)->dport = sport;  // We record connector's sport as our inet_dport.
    inet_sk(newsk)->num = ntohs(dport);
    inet_sk(newsk)->sport = dport;
#else
    inet_sk(newsk)->inet_dport = sport;  // We record connector's sport as our inet_dport.
    inet_sk(newsk)->inet_num = ntohs(dport);
    inet_sk(newsk)->inet_sport = dport;
#endif
    newsk->sk_write_space = sk_stream_write_space;

    newicsk->icsk_retransmits = 0;
    newicsk->icsk_backoff = 0;
    newicsk->icsk_probes_out = 0;
    /* Deinitialize accept_queue to trap illegal accesses. */
    memset(&newicsk->icsk_accept_queue, 0, sizeof(newicsk->icsk_accept_queue));
    return newsk;
}



// Modeled on tcp_create_openreq_child.
struct sock *tcp_forge_openreq_child(struct sock *sk, struct sock_state *st) {
    struct inet_connection_sock *newicsk;
    struct tcp_sock *newtp, *oldtp;
    struct sock *newsk;

    newsk = inet_csk_forge(sk, st->sport, st->dport,
                         GFP_ATOMIC);
    if (newsk == NULL) {
        return NULL;
    }

    newicsk = inet_csk(newsk);
    newtp = tcp_sk(newsk);
    oldtp = tcp_sk(sk);

#ifndef LINUX_PRE_2_6_32
    BUG_ON(oldtp->cookie_values != NULL);
#endif
    newtp->pred_flags = 0;
    newtp->rcv_wup = newtp->copied_seq = newtp->rcv_nxt = st->rcv_nxt;

    newtp->snd_sml = newtp->snd_nxt = newtp->snd_up = st->snd_nxt; /* + tcp_s_data_size(oldtp) */

    tcp_prequeue_init(newtp);  // TODO(swolchok): check this.

    newtp->srtt = 0;
    newtp->mdev = TCP_TIMEOUT_INIT;
    newicsk->icsk_rto = TCP_TIMEOUT_INIT;

    newtp->packets_out = 0;
    newtp->retrans_out = 0;
    newtp->sacked_out = 0;
    newtp->fackets_out = 0;
    newtp->snd_ssthresh = TCP_INFINITE_SSTHRESH;

    newtp->snd_cwnd = 2;
    newtp->snd_cwnd_cnt = 0;
    newtp->bytes_acked = 0;

    newtp->frto_counter = 0;
    newtp->frto_highmark = 0;

    newicsk->icsk_ca_ops = &tcp_init_congestion_ops;
  
    tcp_set_ca_state(newsk, TCP_CA_Open);
    tcp_init_xmit_timers(newsk);
    skb_queue_head_init(&newtp->out_of_order_queue);
    newtp->write_seq = newtp->pushed_seq = newtp->snd_nxt;
  
    newtp->rx_opt.saw_tstamp = 0;

    newtp->rx_opt.dsack = 0;
    newtp->rx_opt.num_sacks = 0;

    newtp->urg_data = 0;

    if (sock_flag(newsk, SOCK_KEEPOPEN))
        inet_csk_reset_keepalive_timer(newsk,
                                   keepalive_time_when(newtp));

    newtp->rx_opt.tstamp_ok = st->tstamp_ok ? 1 : 0;
    if ((newtp->rx_opt.sack_ok = st->sack_ok) != 0) {
        if (sysctl_tcp_fack) {
            tcp_enable_fack(newtp);
        }
    }

    // Just set it to the max. (TODO: fix)
    newtp->window_clamp = 65535 << 14;
    newtp->rcv_ssthresh = newtp->rcv_wnd = st->rcv_wnd << st->rcv_wscale;
    newtp->rx_opt.wscale_ok = st->wscale_ok;
    if (newtp->rx_opt.wscale_ok) {
        newtp->rx_opt.snd_wscale = st->snd_wscale;
        newtp->rx_opt.rcv_wscale = st->rcv_wscale;
    } else {
        newtp->rx_opt.snd_wscale = newtp->rx_opt.rcv_wscale = 0;
        newtp->window_clamp = min(newtp->window_clamp, 65535U);
    }

    newtp->snd_wnd = st->snd_wnd << newtp->rx_opt.snd_wscale;
    newtp->max_window = newtp->snd_wnd;

    if (newtp->rx_opt.tstamp_ok) {
        newtp->rx_opt.ts_recent = st->ts_recent;
        newtp->rx_opt.ts_recent_stamp = get_seconds();
        // We want get_seconds() + ts_offset == st->ts_val.
        //newtp->rx_opt.ts_offset = st->ts_val - tcp_time_stamp;
        newtp->rx_opt.rcv_tsval = st->ts_val;

        newtp->tcp_header_len = sizeof(struct tcphdr) + TCPOLEN_TSTAMP_ALIGNED;
    } else {
        newtp->rx_opt.ts_recent_stamp = 0;
        newtp->tcp_header_len = sizeof(struct tcphdr);
        //newtp->rx_opt.ts_offset = 0;
        newtp->rx_opt.rcv_tsval = 0;
    }

    // XXX: I don't know what this is...
#if 0
    if (skb->len >= TCP_MSS_DEFAULT + newtp->tcp_header_len)
        newicsk->icsk_ack.last_seg_size = skb->len - newtp->tcp_header_len;
#endif
    newtp->rx_opt.mss_clamp = st->mss_clamp;
    newtp->ecn_flags = st->ecn_ok ? TCP_ECN_OK : 0;
    return newsk;
}
EXPORT_SYMBOL(tcp_forge_openreq_child);

// Modeled on tcp_v4_syn_recv_sock.
struct sock *tcp_v4_forge_syn_recv_sock(struct sock *sk, struct sock_state *st) {
    struct dst_entry *dst;
    struct inet_sock *newinet;
    struct tcp_sock *newtp;
    struct sock *newsk;
    struct rtable *rt;
    if ((rt = fake_inet_csk_route_req(sk, st)) == NULL) {
        return NULL;
    }
    //dst = &rt->dst;
    dst = &rt->u.dst;
  
    newsk = tcp_forge_openreq_child(sk, st);
    if (!newsk) {
        goto exit_nonewsk;
    }
  
    newsk->sk_gso_type = SKB_GSO_TCPV4;
    sk_setup_caps(newsk, dst);
    newtp = tcp_sk(newsk);
    newinet = inet_sk(newsk);
#ifdef LINUX_PRE_2_6_32
    newinet->daddr = st->saddr;
    newinet->rcv_saddr = st->daddr;
    newinet->saddr = st->daddr;
    newinet->id = newtp->write_seq ^ jiffies;
#else
    newinet->inet_daddr = st->saddr;
    newinet->inet_rcv_saddr = st->daddr;
    newinet->inet_saddr = st->daddr;
    newinet->inet_id = newtp->write_seq ^ jiffies;
#endif
    // TODO(swolchok): do I have to kmalloc a struct ip_options?
    newinet->opt = NULL;
    newinet->mc_index = rt->rt_iif; // Check this...should be inet_iif(skb).
    newinet->mc_ttl = 1;  // We are not going to multicast.
    inet_csk(newsk)->icsk_ext_hdr_len = 0;
  
    // This should all work fine for us.
    tcp_mtup_init(newsk);
    tcp_sync_mss(newsk, dst_mtu(dst));
    newtp->advmss = dst_metric(dst, RTAX_ADVMSS);
    if (tcp_sk(sk)->rx_opt.user_mss &&
        tcp_sk(sk)->rx_opt.user_mss < newtp->advmss) {

        newtp->advmss = tcp_sk(sk)->rx_opt.user_mss;
    }
  
    tcp_initialize_rcv_mss(newsk);
    return newsk;
  
exit_nonewsk:
    dst_release(dst);
    return NULL;
}
EXPORT_SYMBOL(tcp_v4_forge_syn_recv_sock);

void fake_recved_ack(struct sock *sk, struct sock_state *st) {
    struct tcp_sock *tp = tcp_sk(sk);
    struct inet_connection_sock *icsk = inet_csk(sk);

    // Next couple lines from step 5 in tcp_rcv_state_process (tcp_input.c).
    tp->copied_seq = tp->rcv_nxt;
    smp_mb();  // Why not?
    tcp_set_state(sk, TCP_ESTABLISHED);  // We have to fake this.
    sk->sk_state_change(sk);

    tp->snd_una = tp->snd_up = st->snd_una;
    tcp_init_wl(tp, st->rcv_nxt);

    // TODO(swolchok): use a real RTT measurement.
    tcp_valid_rtt_meas(sk, msecs_to_jiffies(10));
    //tcp_ack_update_rtt(sk, 0, 0);

    if (tp->rx_opt.tstamp_ok)
        tp->advmss -= TCPOLEN_TSTAMP_ALIGNED;

    icsk->icsk_af_ops->rebuild_header(sk);

    tcp_init_metrics(sk);

    tcp_init_congestion_control(sk);

    tp->lsndtime = tcp_time_stamp;

    tcp_mtup_init(sk);
    tcp_initialize_rcv_mss(sk);
    tcp_init_buffer_space(sk);
    tcp_fast_path_on(tp);
}

// The returned socket is bh_lock_sock()ed, because of sk_clone.
// This function will fake a received SYN and ACK (normally, we'd 
// send a SYN+ACK in there).
struct sock *fake_inet_csk_accept(struct sock *sk, int flags, int *err,
                                  struct sock_state *st) {
    struct sock *newsk;

    lock_sock(sk);

    // We're not going to check the socket state or the accept queue. Instead...
    newsk = tcp_v4_forge_syn_recv_sock(sk, st);
    if (!newsk) {
        *err = -EINVAL;
        release_sock(sk);   // Added...pretty sure we need this here
        return NULL;
    }

    fake_recved_ack(newsk, st);

    local_bh_disable();
    __inet_inherit_port(sk, newsk);
    /*if (__inet_inherit_port(sk, newsk) < 0) {
        sock_put(newsk);
        *err = -EINVAL;
        goto exit;
    }*/

#ifdef LINUX_PRE_2_6_32
    __inet_hash_nolisten(newsk);
#else
    __inet_hash_nolisten(newsk, NULL);
#endif
    release_sock(sk);
    local_bh_enable();

    return newsk;
/*
exit:
    NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_LISTENDROPS);
    local_bh_enable();
    return NULL;
*/

}

int fake_inet_accept(struct socket *sock, struct socket *newsock, int flags,
                     struct sock_state *st) {
    struct sock *sk1 = sock->sk;
    int err = -EINVAL;
    // This is our only change from inet_accept.
    struct sock *sk2 = fake_inet_csk_accept(sk1, flags, &err, st);

    if (!sk2) {
        goto do_err;
    }

    //  lock_sock(sk2);  // It's already locked.
    sock_graft(sk2, newsock);
    newsock->state = SS_CONNECTED;
    err = 0;
    bh_unlock_sock(sk2);
    BUG_ON(atomic_read(&sk2->sk_refcnt) < 2);
    sock_put(sk2);  // Do we need this? Check for socket leak later (not now).

do_err:
    return err;
}
EXPORT_SYMBOL(fake_inet_accept);

int fake_accept_ioctl(struct sock *sk, unsigned long arg) {
        struct sock_state st;
        struct socket *sock, *newsock;
        int newfd, err;
        sock = sk->sk_socket;

        if (optlen != sizeof(st)) {
            return -EINVAL;
        }

        if (copy_from_user(&st, (struct sock_state __user *)optval, sizeof(st))) {
            return -EFAULT;
        }





        err = sock_create_lite(sk->sk_family, sk->sk_type, sk->sk_protocol, &newsock);
        if (err < 0) {
            return err;
        }

        WARN_ON(!(newsock->type == sock->type));
        newsock->ops = sock->ops;

        __module_get(newsock->ops->owner);

        newfd = sock_map_fd(newsock, 0);
        if (unlikely(newfd < 0)) {
            sock_release(newsock);
            return newfd;
        }

        err = fake_inet_accept(sock, newsock, sock->file->f_flags, &st);
        if (err < 0) {
            sys_close(newfd);
            printk(KERN_DEBUG "fake_accept_ioctl close and fail\n");
            return err;
        }

        // NOTE: we omitted a check on upeer_sockaddr here.
        err = newfd;
        return err;



}
EXPORT_SYMBOL(fake_accept_ioctl);


#endif

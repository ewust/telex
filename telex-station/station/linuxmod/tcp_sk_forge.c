/* Copyright 2012, 2013, 2014  University of Michigan

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <linux/atomic.h>
#include <linux/file.h>
#include <linux/jiffies.h>
#include <linux/syscalls.h>
#include <linux/types.h>
#include <linux/tcp.h>
#include <linux/tcp_sk_forge.h>
#include <net/inet_connection_sock.h>
#include <net/tcp.h>
#include <net/tcp_states.h>

#include <asm/uaccess.h>
/* sk should be the struct sock from our original struct socket passed to accept. */

struct sock *inet_csk_forge(struct sock *sk, __be16 sport, __be16 dport,  const gfp_t priority) {
  struct sock *newsk = sk_clone(sk, priority);
  struct inet_connection_sock *newicsk = inet_csk(newsk);

  if (newsk == NULL) {
    return newsk;
  }

  newsk->sk_state = TCP_SYN_RECV;
  inet_sk(newsk)->inet_dport = sport;  // We record connector's sport as our inet_dport.
  inet_sk(newsk)->inet_num = ntohs(dport);
  inet_sk(newsk)->inet_sport = dport;
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

  BUG_ON(oldtp->cookie_values != NULL);
  newtp->pred_flags = 0;
  newtp->rcv_wup = newtp->copied_seq =
    newtp->rcv_nxt = st->rcv_nxt;

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
  newtp->write_seq = newtp->pushed_seq =
    newtp->snd_nxt;
  
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

  // Just set it to the max.
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
    newtp->rx_opt.ts_offset = st->ts_val - tcp_time_stamp;

    newtp->tcp_header_len = sizeof(struct tcphdr) + TCPOLEN_TSTAMP_ALIGNED;
  } else {
    newtp->rx_opt.ts_recent_stamp = 0;
    newtp->tcp_header_len = sizeof(struct tcphdr);
    newtp->rx_opt.ts_offset = 0;
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

// Unlike the real inet_csk_route_req, we return the rtable.
struct rtable *fake_inet_csk_route_req(struct sock *sk, struct sock_state *st) {
  struct rtable *rt;
  // We don't care about IP options...
  struct flowi fl = {.oif = sk->sk_bound_dev_if,
                     .mark = sk->sk_mark,
                     .nl_u = { .ip4_u =
                               { .daddr = st->saddr,
                                 .saddr = st->daddr,
                                 .tos = 0 }}, // TOS? DGAF.
                     .proto = sk->sk_protocol,
                     .flags = inet_sk_flowi_flags(sk),
                     .uli_u = { .ports =
                                { .sport = st->dport,
                                  .dport = st->sport } } };
  struct net *net = sock_net(sk);
  if (ip_route_output_flow(net, &rt, &fl, sk, 0)) {
    IP_INC_STATS_BH(net, IPSTATS_MIB_OUTNOROUTES);
    return NULL;
  }
  return rt;
}
EXPORT_SYMBOL(fake_inet_csk_route_req);

// Modeled on tcp_v4_syn_recv_sock.
struct sock *tcp_v4_forge_syn_recv_sock(struct sock *sk, struct sock_state *st) {
  struct dst_entry *dst;
  struct inet_sock *newinet;
  struct tcp_sock *newtp;
  struct sock *newsk;
  struct rtable *rt;
  if ((rt = fake_inet_csk_route_req(sk, st)) == NULL) {
    goto exit_nonewsk;
  }
  dst = &rt->dst;
  newsk = tcp_forge_openreq_child(sk, st);
  if (!newsk) {
    goto exit_nonewsk;
  }

  newsk->sk_gso_type = SKB_GSO_TCPV4;
  sk_setup_caps(newsk, dst);
  newtp = tcp_sk(newsk);
  newinet = inet_sk(newsk);
  newinet->inet_daddr = st->saddr;
  newinet->inet_rcv_saddr = st->daddr;
  newinet->inet_saddr = st->daddr;
  // TODO(swolchok): do I have to kmalloc a struct ip_options?
  newinet->opt = NULL;
  newinet->mc_index = rt->rt_iif; // Check this...should be inet_iif(skb).
  newinet->mc_ttl = 1;  // We are not going to multicast.
  inet_csk(newsk)->icsk_ext_hdr_len = 0;
  newinet->inet_id = newtp->write_seq ^ jiffies;

  // This should all work fine for us.
  tcp_mtup_init(newsk);
  tcp_sync_mss(newsk, dst_mtu(dst));
  newtp->advmss = dst_metric(dst, RTAX_ADVMSS);
  if (tcp_sk(sk)->rx_opt.user_mss &&
      tcp_sk(sk)->rx_opt.user_mss < newtp->advmss)
    newtp->advmss = tcp_sk(sk)->rx_opt.user_mss;

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
//  tcp_ack_update_rtt(sk, 0, 0);

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
struct sock *fake_inet_csk_accept(struct sock *sk, int flags, int *err,
                                  struct sock_state *st) {
  struct sock *newsk;

  lock_sock(sk);
  // We're not going to check the socket state or the accept queue. Instead...
  newsk = tcp_v4_forge_syn_recv_sock(sk, st);
  if (!newsk) {
    *err = -EINVAL;
    return NULL;
  }

  fake_recved_ack(newsk, st);

  local_bh_disable();
  if (__inet_inherit_port(sk, newsk) < 0) {
    sock_put(newsk);
    *err = -EINVAL;
    goto exit;
  }

  __inet_hash_nolisten(newsk, NULL);
  release_sock(sk);
  local_bh_enable();

  return newsk;

exit:
  NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_LISTENDROPS);
  local_bh_enable();
  return NULL;

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
  if (copy_from_user(&st, (struct sock_state __user *)arg, sizeof(st))) {
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
    goto out_fd;
  }

  // NOTE: we omitted a check on upeer_sockaddr here.
  err = newfd;
  return err;
out_fd:
  sys_close(newfd);
  printk(KERN_DEBUG "fake_accept_ioctl close and fail\n");
  return err;
}
EXPORT_SYMBOL(fake_accept_ioctl);

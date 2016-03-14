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

#ifndef _TCP_SK_FORGE_H
#define _TCP_SK_FORGE_H

struct sock;
int fake_accept_ioctl(struct sock *sk, unsigned long arg);

// TODO(swolchok): go back and check if we actually use all the
// members we are capable of taking here.
struct sock_state {
  // Source = remote side, because they're sending the SYN.
  __be16 sport;
  __be16 dport;
  __be32 saddr;
  __be32 daddr;


  __u32 mss_clamp; /* XXX: I guess this is negotiated in the handshake? Eric? */

  __u32	rcv_nxt;	/* What we want to receive next 	*/
  __u32	rcv_wup;	/* rcv_nxt on last window update sent	*/
  __u32	snd_nxt;	/* Next sequence we send		*/
  
  __u32	snd_una;	/* First byte we want an ack for. SET THIS! */
  __u32	rcv_tstamp;	/* timestamp of last received ACK (for keepalives) */

  __u32	snd_wnd;	/* The window we expect to receive SET THIS! */
  __u32	mss_cache;	/* Cached effective mss, not including SACKS */

  __u16	advmss;		/* Advertised MSS			*/

  // Omitting all the stuff below frto_counter in struct
  // tcp_sock. It's just too much.
  __u32 ts_recent;  // Timestamp to echo next.
  __u32 ts_val;   // Timestamp to use next.
  __u8 tstamp_ok;  // Did SYN packet have timestamp?
  __u8 sack_ok;  // Did SYN packet have SACK? (heh heh)
  __u8 wscale_ok;  // Did SYN packet have Wscale?
  __u8 snd_wscale;  // Set if wscale_ok.
  __u8 rcv_wscale;  // Ditto.
  __u8 ecn_ok;  // DO YOU WANT ECN?

  __u32 rcv_wnd;
};

#ifdef __KERNEL__
// XXX: from tcp_input.c.
void tcp_ack_update_rtt(struct sock *sk, const int flag,
                        const s32 seq_rtt);
void tcp_init_metrics(struct sock *sk);
void tcp_init_buffer_space(struct sock *sk);
void tcp_valid_rtt_meas(struct sock *sk, u32 seq_rtt);
#endif


#endif /* _TCP_SK_FORGE_H */

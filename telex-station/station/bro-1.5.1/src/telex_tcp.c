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

#include "telex_tcp.h"

int raw_sock;

void telex_tcp_init(void)
{
    raw_sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (raw_sock == -1) {
        perror("couldn't open socket");
        exit(1);
    }
    return;
}

void print_flow(struct flow *fl)
{

    char sip[INET_ADDRSTRLEN];
    char dip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &fl->orig_h, sip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &fl->resp_h, dip, INET_ADDRSTRLEN);
       
    printf("%s:%d -> %s:%d\n", sip, ntohs(fl->orig_p), dip, ntohs(fl->resp_p)); 

}


uint16_t csum(uint16_t *buf, int nwords, uint32_t init_sum)
{
    uint32_t sum;

    for (sum=init_sum; nwords>0; nwords--) {
        sum += ntohs(*buf++);
    }
    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);
    return (uint16_t)(~sum);
}

uint16_t tcp_csum(struct iphdr *ip_hdr)
{
    uint32_t sum = 0;
    int tcp_size = ntohs(ip_hdr->tot_len) - sizeof(struct iphdr);
    struct tcphdr *tcph = IP_TCP(ip_hdr);

    sum += ntohs(ip_hdr->saddr&0xffff) + ntohs((ip_hdr->saddr >> 16)&0xffff);
    sum += ntohs(ip_hdr->daddr&0xffff) + ntohs((ip_hdr->daddr >> 16)&0xffff);
    sum += 0x06;    // TCP protocol #define somewhere(?), plz
    sum += tcp_size;
    //printf ("init csum: %x, tcp size: %d\n", sum, tcp_size);

    tcph->th_sum = 0x0000;

    if (tcp_size%2) { //odd tcp_size,
        sum += (((char*)tcph)[tcp_size-1] << 8);
    } 

    return csum((uint16_t*)tcph, tcp_size/2, sum);
}

//given two two timevals, determines the difference
//in microseconds
uint64_t telex_get_tv_diff(struct timeval *a, struct timeval *b)
{
    uint64_t ret;
    int32_t usec_diff, sec_diff;
    
    sec_diff = b->tv_sec - a->tv_sec;
    usec_diff = b->tv_usec - a->tv_usec;
    
    ret = (uint64_t)(((int64_t)sec_diff)*1000000L + (int64_t)usec_diff);
return ret;
}

uint32_t telex_get_next_tsval(struct telex_tcp_ts *last, struct telex_tcp_ts *cur)
{
    uint64_t tv_diff, ts_diff;
    uint32_t us_per_tsval;
    struct timeval tv;
    uint32_t ret;
    static uint16_t warned_tsval = 0;

    //difference in true time (microseconds)
    tv_diff = telex_get_tv_diff(&last->tv, &cur->tv);
    
    //difference in tcp timestamp val
    ts_diff = cur->tsval - last->tsval; 

    if (ts_diff == 0) {
        if (warned_tsval++ == 0)
            printf("Warning: tsval difference of 0\n");
        return cur->tsval;
    }
    
    //microseconds per TCP tsval
    us_per_tsval = (uint32_t)(tv_diff / ts_diff);


    //extrapolate
    gettimeofday(&tv, NULL);
    tv_diff = telex_get_tv_diff(&cur->tv, &tv);

    ret = cur->tsval + (tv_diff / us_per_tsval);

    return ret;
}

int telex_fill_ts(struct telex_tcp_info *tcp_info, struct tcphdr *tcp_hdr)
{
    struct tcp_option_ts ts;
    uint32_t next_tsval;


    next_tsval = telex_get_next_tsval(&tcp_info->last_ts, &tcp_info->cur_ts);
    
    ts.nops = 0x0101;
    ts.op_type = 0x08;
    ts.op_len = 0x0a;
    ts.tsval = htonl(next_tsval);
    ts.tsecr = htonl(tcp_info->cur_ts.tsecr);

    memcpy((char*)(tcp_hdr+1), &ts, sizeof(ts));

    return 0;
}


//warning: unlikely integer overflow on tot_len
//maybe place reasonable constraint on len (<= 10000, jumbo frames?)
int telex_tcp_forge_xmit(struct flow *fl, char *payload, int len)
{
    int option_ts = fl->tcp.option_ts;
    int tcp_len = sizeof(struct tcphdr) + 
                  (option_ts ? sizeof(struct tcp_option_ts) : 0);

    char *forge_again = NULL;
    int forge_again_len = 0;
    if (len > 1448) {   // TODO(ewust): #define, please.
                        // TODO(ewust): this is ETH_MTU - sizeof(ip) - sizeof(tcp)
                        //              which are variable.
        forge_again = payload + 1448;
        forge_again_len = len - 1448;
        len = 1448;
    }

    int tot_len = len + tcp_len + sizeof(struct iphdr);
    struct iphdr *ip_hdr;
    struct tcphdr *tcp_hdr;
    char *data;
    struct sockaddr_in sin;
    int res; 
    
    //set up sin destination    
    sin.sin_family = AF_INET;
    sin.sin_port = (fl->orig_p);
    sin.sin_addr.s_addr = (fl->orig_h);
   
    
    ip_hdr = malloc(tot_len);
    if (ip_hdr == NULL) {
        return 1;
    }

    //zero-fill headers
    memset(ip_hdr, 0, sizeof(struct iphdr) + tcp_len);

    //no ip options
    tcp_hdr = (struct tcphdr*)(ip_hdr+1);
    data = (char *)(tcp_hdr) + tcp_len;
     
    //copy payload data
    if (len != 0) {
        memcpy(data, payload, len);
    }

    //fill in ip header
    ip_hdr->ihl         = sizeof(struct iphdr) >> 2;
    ip_hdr->version     = 4;
    ip_hdr->tot_len     = htons(tot_len);
    ip_hdr->frag_off    = htons(0x4000); //don't fragment
    ip_hdr->ttl         = fl->last_ttl; 
    ip_hdr->id          = htons(fl->last_ip_id+1); 
    ip_hdr->protocol    = IPPROTO_TCP;
    ip_hdr->saddr       = (fl->resp_h);
    ip_hdr->daddr       = (fl->orig_h);

    //fill in tcp header
    tcp_hdr->th_sport   = (fl->resp_p);
    tcp_hdr->th_dport   = (fl->orig_p);
    tcp_hdr->th_seq     = htonl(fl->tcp.seq);
    tcp_hdr->th_ack     = htonl(fl->tcp.ack);
    tcp_hdr->th_off     = tcp_len >> 2;
    tcp_hdr->th_flags   = TH_ACK;
    if (len != 0) {
        tcp_hdr->th_flags |= TH_PUSH; //0x18; //PSH + ACK
    }
    if (fl->tcp.flags != 0) {
        tcp_hdr->th_flags = fl->tcp.flags;
    }
    tcp_hdr->th_win     = htons(fl->tcp.snd_window);

    if (option_ts)
        telex_fill_ts(&fl->tcp, tcp_hdr);

    tcp_hdr->th_sum = htons(tcp_csum(ip_hdr));
    ip_hdr->check = htons(csum((uint16_t*)ip_hdr, sizeof(struct iphdr)/2, 0));

    res = sendto(raw_sock, ip_hdr, tot_len, 0, (struct sockaddr*)&sin, sizeof(sin));

    if (forge_again == NULL) {  // Normal path, our data fits in one packet.
        free(ip_hdr);
        return (res != tot_len);
    }

    fl->tcp.seq += len;
    fl->last_ip_id++;
    // TODO(ewust): does window size shrink here?
    //      If so, we would need to know about window scaling.

    res = telex_tcp_forge_xmit(fl, forge_again, forge_again_len);

    //set fl back to how it was, caller will likely want to increment in the normal case
    fl->tcp.seq -= len;

    free(ip_hdr);
     
    return res;  
}

int telex_fwd_pkt(char *pkt, int len)
{
    int res;
    struct sockaddr_in sin;
    struct iphdr *ip = (struct iphdr *)pkt;
    struct tcphdr *tcp = IP_TCP(ip);
    int old_len = len;

    //set up sin destination    
    sin.sin_family = AF_INET;
    sin.sin_port = (tcp->th_dport);
    sin.sin_addr.s_addr = (ip->daddr);

#if 0
    // If we are more than MTU, just send MTU so it's not
    // a complete packet drop. This is of course a great hack,
    // but as long as Linux keeps defragmenting our packets
    // off the network card and NOT supporting jumbo frames (seriously?!?)
    // this hack will do better than not.
    if (len > 1500) {
        //printf("truncating %d byte packet to 1500 bytes\n", len);
        len = 1500;
        ip->tot_len = htons(1500);
        if (tcp->th_flags & TH_PUSH) {
            psh = 1;
            tcp->th_flags &= ~TH_PUSH;
        }
        tcp->th_sum = htons(tcp_csum(ip));  // Recalc tcp csum
    }
#endif 

    // Increment TTL by 1, so going through the router twice is not detectable
    ip->ttl++; 
    ip->check = htons(csum((uint16_t*)ip, (ip->ihl * 2), 0)); 

    res = sendto(raw_sock, pkt, len, 0, (struct sockaddr*)&sin, sizeof(sin));
    if (res != len) {
        perror("Error: telex_fwd_pkt ");
        printf("    tried to send %d, sendto returned %d\n", len, res);
    } else if (old_len != len) {
#if 0
        // construct another. gah fragments.
        int len_diff = old_len - len;
        char *payload = TCP_DATA(tcp);
        int headers_len = (int)(payload - pkt);
 
        if (len_diff <= 0) {
            printf("Error: telex_fwd_pkt: old_len %d, len: %d, diff: %d\n", old_len, len, len_diff);
            return 1; 
        } 

        ip->id = htons(ntohs(ip->id) + 1);
        ip->tot_len = htons(len_diff + headers_len);
        
        memmove(payload, &pkt[len], len_diff);
        
        // Update tcp params
        tcp->th_seq = htonl(ntohl(tcp->th_seq) + (len - headers_len));
        
        if (psh) {
            tcp->th_flags |= TH_PUSH; 
        }
        tcp->th_sum = htons(tcp_csum(ip));
        // No ip csum needed, since pkt_fwd will just recompute it, since it adds one to ttl
        // Oh, which reminds me..
        ip->ttl--;
        // :) I am going to hate myself if I ever have to read this code again!
        return telex_fwd_pkt(pkt, len_diff + headers_len);
#endif
    }

    return (res != len);
}

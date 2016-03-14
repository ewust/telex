#include "libaddipportip.h"



void telex_local_mod_flow(const void *src_in_addr, uint16_t sport,
                          const void *dest_in_addr, uint16_t dport,
                          int address_family, int block);

void telex_block_flow_init(void);


void send_iptables_flow_command(char sip[INET_ADDRSTRLEN], uint16_t sport, 
                                       char dip[INET_ADDRSTRLEN], uint16_t dport,
                                       int address_family, int block);


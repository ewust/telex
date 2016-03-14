#ifndef TELEX_BLOCK_FLOW_H
#define TELEX_BLOCK_FLOW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


#define BLOCK_FLOW_MSG_LENGTH (1 + 4 + 2 + 4 + 2)
#define REMOTE_BLOCK_FLOW

#define BLOCK 1
#define UNBLOCK 0
int telex_mod_flow(const uint32_t saddr, uint16_t sport,
                     const uint32_t daddr, uint16_t dport, int block);
void telex_local_mod_flow(const void *src_in_addr, uint16_t sport,
                          const void *dest_in_addr, uint16_t dport,
                          int address_family, int block);
int telex_remote_mod_flow(const void *src_in_addr, uint16_t sport,
                          const void *dest_in_addr, uint16_t dport,
                          int address_family, int block);
int telex_connect_to_flow_blocker(const char *ip, const char *port,
                                  const char **error_str);

void telex_block_flow_init(void);


#ifdef __cplusplus
}
#endif


#endif /* TELEX_BLOCK_FLOW_H */

#include "telex_block_flow.h"
#include "telex_util.h"
#include "telex_ipset.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static int flow_blocker_sock;

int telex_remote_mod_flow(const void *src_in_addr, uint16_t sport,
                                 const void *dest_in_addr, uint16_t dport,
                                 int address_family, int block) {
  assert(address_family == AF_INET);

  char msg[BLOCK_FLOW_MSG_LENGTH];
  int ip_size = 4;
  char *p = msg;
  
  *p++ = block ? 1 : 0;

  memcpy(p, src_in_addr, ip_size);
  p += ip_size;

  memcpy(p, &sport, sizeof(sport));
  p += sizeof(sport);

  memcpy(p, dest_in_addr, ip_size);
  p += ip_size;

  memcpy(p, &dport, sizeof(dport));
  p += sizeof(dport);

  assert((p - msg)  == BLOCK_FLOW_MSG_LENGTH);
  /* TODO(swolchok): error check. */
  return sendall(flow_blocker_sock, msg, p - msg);
}


/* Returns connected socket on success, negative error code otherwise.
   error_str may be NULL; if not, *error_str will point to a string containing
   an appropriate message on error.
   Not re-entrant.
*/
int telex_connect_to_flow_blocker(const char *ip, const char *port,
                                  const char **error_str) {
  struct addrinfo *info = NULL;
  struct addrinfo hints = {0};
  int err_code;

  //telex_block_flow_init();  //is this needed for the bronnect/bro side?
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_NUMERICHOST | AI_NUMERICSERV | AI_ADDRCONFIG;

  if ((err_code = getaddrinfo(ip, port, &hints, &info))) {
    if (error_str) {
      *error_str = gai_strerror(err_code);
    }
    return err_code;
  }

  if ((flow_blocker_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    goto ERRNO_ERROR;
  }

  if (connect(flow_blocker_sock, info->ai_addr, info->ai_addrlen) == -1) {
    goto ERRNO_ERROR;
  }

  if (info) {
    freeaddrinfo(info);
  }
  return flow_blocker_sock;
ERRNO_ERROR:
  if (info) {
    freeaddrinfo(info);
  }

  if (error_str) {
    *error_str = strerror(errno);
  }
  return errno;
}

/* TODO: error handling. */
int telex_mod_flow(const uint32_t saddr, uint16_t sport, 
                     const uint32_t daddr, uint16_t dport, int block)
{
	//iptables -A INPUT -s 68.40.59.167 -d 141.212.202.168 -p tcp --dport 45804 --sport 4501 -j DROP

#ifdef REMOTE_BLOCK_FLOW
    return telex_remote_mod_flow(&saddr, sport,
                                 &daddr, dport, AF_INET, block);
#else
    telex_local_mod_flow(&saddr, sport,
                         &daddr, dport, AF_INET, block);
    return 0;
#endif
}


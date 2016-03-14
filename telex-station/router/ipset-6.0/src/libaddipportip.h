#ifndef LIBADDIPPORTIP_H
#define LIBADDIPPORTIP_H

#include <netinet/in.h>

void libaddipportip_init(void);
void libaddipportip_fini(void);
int add_ipportip(const char *setname, const char ip1[INET_ADDRSTRLEN],
                 uint16_t port, const char ip2[INET_ADDRSTRLEN],
                 int block);
#endif /* LIBADDIPPORTIP_H */

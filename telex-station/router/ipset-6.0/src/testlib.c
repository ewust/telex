#include "libaddipportip.h"
#include <stdio.h>
int main(void) {
  libaddipportip_init();
  fprintf(stderr, "%d\n",
          add_ipportip("OURSET", "1.2.3.4", 6969, "5.6.7.8", 1));
  libaddipportip_fini();
  return 0;
}

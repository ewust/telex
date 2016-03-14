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

#include "libaddipportip.h"

#include <libipset/session.h>
#include <libipset/types.h>

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

extern struct ipset_type ipset_hash_ipportip0;
static struct ipset_session *session = NULL;

void libaddipportip_init(void) {
  ipset_type_add(&ipset_hash_ipportip0);
  session = ipset_session_init(printf);
  if (session == NULL) {
    fprintf(stderr, "Can't init ipset session, bye.\n");
    exit(-1);
  }
}

static int handle_error(void) {

  if (ipset_session_warning(session) 
      && !ipset_envopt_test(session, IPSET_ENV_QUIET)) {
    fprintf(stderr, "Warning: %s\n", ipset_session_warning(session));
    return 0;
  }

  if (ipset_session_error(session)) {
    fprintf(stderr, "Error: %s\n", ipset_session_error(session));
    ipset_session_report_reset(session);
    return 1;
  }

  ipset_session_fini(session);
  exit(-1);

  /*
  if (session) {
    ipset_session_fini(session);
    session = NULL;
  }
  return -1;
  */
}

void libaddipportip_fini(void) {
  if (session) {
    ipset_session_fini(session);
  }
}

int add_ipportip(const char *setname, const char ip1[INET_ADDRSTRLEN],
                 uint16_t port, const char ip2[INET_ADDRSTRLEN],
                 int block) {
  int cmd = block ? IPSET_CMD_ADD : IPSET_CMD_DEL;
  char ipset_fake_arg[INET_ADDRSTRLEN * 2 + 5 + 1];
  int r;
  const struct ipset_type *type;
  sprintf(ipset_fake_arg, "%s,%d,%s", ip1, port, ip2);
  r = ipset_parse_setname(session, IPSET_SETNAME, setname);
  if (r < 0) {
    return handle_error();
  }

  type = ipset_type_get(session, cmd);
  if (type == NULL) {
    return handle_error();
  }

  r = ipset_parse_elem(session, type->last_elem_optional,
                           ipset_fake_arg);
  if (r < 0) {
    return handle_error();
  } else if (r) {
    return r;
  }

  r = ipset_cmd(session, cmd, 0);
  if (r < 0) {
    handle_error();
  }
  return r;
}

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

#include "telex_block_flow.h"
#include "telex_ipset.h"


void telex_local_mod_flow(const void *src_in_addr, uint16_t sport,
                          const void *dest_in_addr, uint16_t dport,
                          int address_family, int block) {
	char sip[INET_ADDRSTRLEN];
	char dip[INET_ADDRSTRLEN];

	inet_ntop(address_family, src_in_addr, sip, INET_ADDRSTRLEN);
	inet_ntop(address_family, dest_in_addr, dip, INET_ADDRSTRLEN);
    send_iptables_flow_command(sip, ntohs(sport), dip, ntohs(dport),
                               address_family, block);
}

void telex_block_flow_init(void) {
  libaddipportip_init();
  atexit(libaddipportip_fini);
}

void send_iptables_flow_command(char sip[INET_ADDRSTRLEN], uint16_t sport,
                                       char dip[INET_ADDRSTRLEN], uint16_t dport,
                                       int address_family, int block) {
  char cmd[500];

  add_ipportip("OURSET", dip, sport, sip, block);
#if 0
    char *argv[18] = {"iptables", 0, "FORWARD", "-i", "eth0", "-s", 0, "-d", 0,
                    "-p", "tcp", "--dport", 0, "--sport", 0, "-j", "DROP", 0};
	char dport_str[16];
    char sport_str[16];
    char add_drop;
    if (block) {
      argv[1] = "-A";
    } else {
      argv[1] = "-D";
    }
    argv[6] = dip;
    argv[8] = sip;

    sprintf(dport_str, "%d", dport);
    sprintf(sport_str, "%d", sport);

    argv[12] = sport_str;
    argv[14] = dport_str;

    pid_t childpid = fork();
    int status;
    if (!childpid) {
      execve("/sbin/iptables", argv, NULL);
    } else {
      waitpid(childpid, &status, 0);
    }
#endif
    // Actually, we don't have to block src->dest traffic.
#if 0
    argv[6] = sip;
    argv[8] = dip;
    argv[12] = dport_str;
    argv[14] = sport_str;

    if (!fork()) {
      execve("/sbin/iptables", argv, NULL);
    }
#endif
#if 0
	p += sprintf(p, "iptables -%c FORWARD -i eth0 -s %s -d %s -p tcp --dport %d --sport %d -j DROP; ", // -i eth0
            add_drop, dip, sip, sport, dport);
    sprintf(p, "iptables -%c FORWARD -i eth1 -s %s -d %s -p tcp --dport %d --sport %d -j DROP", // -i eth1
            add_drop, sip, dip, dport, sport);
//	printf("%s\n", cmd);
	if (system(cmd) != 0) {
		printf("Error running iptables\n");
	}
#endif
}


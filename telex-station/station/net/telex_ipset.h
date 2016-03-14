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

#include "libaddipportip.h"



void telex_local_mod_flow(const void *src_in_addr, uint16_t sport,
                          const void *dest_in_addr, uint16_t dport,
                          int address_family, int block);

void telex_block_flow_init(void);


void send_iptables_flow_command(char sip[INET_ADDRSTRLEN], uint16_t sport, 
                                       char dip[INET_ADDRSTRLEN], uint16_t dport,
                                       int address_family, int block);


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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>
#include <linux/types.h>
#include <linux/tcp_sk_forge.h>

int main(void) {
  int s = socket(AF_INET, SOCK_STREAM, 0);
  int conn;
  if (s == -1) {
    perror("socket");
    return 1;
  }

  struct sockaddr_in addr;

  addr.sin_family = AF_INET;
  addr.sin_port = htons(443);
  addr.sin_addr.s_addr = htonl(0x04020201);
  if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind");
    return 2;
  }

  if (setsockopt(s, SOL_SOCKET, SO_BINDTODEVICE, "eth0", 4) < 0) {
    perror("setsockopt");
    return 6;
  }

  int value = 1;
  setsockopt(s, SOL_IP, IP_TRANSPARENT, &value, sizeof(value));

  struct sock_state st = {0};
  st.dport = htons(443);
  st.daddr = htonl(0x04020201);
  st.saddr = htonl(0x8dd46d39);
  st.sport = htons(9999);
  st.rcv_isn = 0x1234;
  st.snd_isn = 0x5678;
  st.snd_una = st.snd_isn + 1;
  st.tstamp_ok = 1;
  st.sack_ok = 0;
  st.rcv_wnd = 1024;
  st.snd_wnd = 1024;

  st.wscale_ok = 0;
  st.ts_recent = 0xDEADBEEF;
  st.mss_clamp = 1234;
  st.ecn_ok = 0;
  if ((conn = ioctl(s, 0xFACE, &st)) < 0) {
    perror("ioctl");
    return 3;
  }
  setsockopt(conn, SOL_IP, IP_TRANSPARENT, &value, sizeof(value));
  if (send(conn, "ZOMG", 4, 0) < 0 ) {
    perror("send zomg failed");
    fflush(stderr);
    fflush(stdout);
    return 4;
  }
  if (close(conn) < 0) {
    perror("close");
    return 5;
  }
  return 0;
}

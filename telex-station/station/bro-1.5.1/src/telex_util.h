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

#ifndef UTIL_H
#define UTIL_H

#include <sys/types.h> // For size_t.

#ifdef __cplusplus
extern "C" {
#endif

int sendall(int sock, const char *buf, size_t len);
void hexdump(void *data, int len);
int start_tcp_connect(char *host, int port, int *need_finish);

// May return < 0 on error; check errno.
int get_port(int sock);
int get_remote_addr(int sock, unsigned long *addr);
#ifdef __cplusplus
}
#endif
#endif /* UTIL_H */

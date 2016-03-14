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

#ifndef TAG_H
#define TAG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ptwist.h"
#include <stdint.h>

#define MAX_CONTEXT_LEN 9   
    // ip(4) + timestamp(4) + session_id(1)

void gen_tag(byte tag[PTWIST_TAG_BYTES], byte key[16],
	const byte *context, size_t context_len);

int check_tag(byte key[16], const byte privkey[PTWIST_BYTES],
	const byte tag[PTWIST_TAG_BYTES], const byte *context,
	size_t context_len);

struct telex_priv_keys {
    byte        key[PTWIST_BYTES];
    uint16_t    port;
} __attribute__((__packed__));

/* Relic and OpenSSL have a name collision at global scope.
   core_init lets us avoid including relic.c in bro.bif. */
void tag_init(void);
void tag_shutdown(void);

void tag_load_pubkey(const char *keyfn);
void tag_load_pubkey_bytes(const char *keybytes, int len);
void tag_load_privkey(const char *keyfn, byte privkey[PTWIST_BYTES]);

#ifdef __cplusplus
}
#endif
#endif

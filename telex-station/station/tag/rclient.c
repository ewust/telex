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

#include <stdio.h>
#include <string.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include "ptwist.h"

byte maingen[PTWIST_BYTES];
byte twistgen[PTWIST_BYTES];
byte mainpub[PTWIST_BYTES];
byte twistpub[PTWIST_BYTES];

static void gen_tag(byte tag[PTWIST_TAG_BYTES], byte key[16],
	const byte *context, size_t context_len)
{
    byte seckey[PTWIST_BYTES];
    byte sharedsec[PTWIST_BYTES+context_len];
    byte usetwist;
    byte taghashout[32];
#if PTWIST_PUZZLE_STRENGTH > 0
    size_t puzzle_len = 16+PTWIST_RESP_BYTES;
    byte value_to_hash[puzzle_len];
    byte hashout[32];
    bn_t Rbn, Hbn;
    int i, len, sign;
#endif

    memset(tag, 0xAA, PTWIST_TAG_BYTES);

    /* Use the main or the twist curve? */
    RAND_bytes(&usetwist, 1);
    usetwist &= 1;

    /* Create seckey*G and seckey*Y */
    RAND_bytes(seckey, PTWIST_BYTES);
    ptwist_pointmul(tag, usetwist ? twistgen : maingen, seckey);
    ptwist_pointmul(sharedsec, usetwist ? twistpub : mainpub, seckey);

    /* Create the tag hash keys */
    memmove(sharedsec+PTWIST_BYTES, context, context_len);
    SHA256(sharedsec, PTWIST_BYTES+context_len, taghashout);

#if PTWIST_PUZZLE_STRENGTH > 0
    /* The puzzle is to find a response R such that SHA256(K || R)
       starts with PTWIST_PUZZLE_STRENGTH bits of 0s.  K is the first
       128 bits of the above hash tag keys. */

    /* Construct our response to the puzzle.  Start looking for R in a
     * random place. */
    memmove(value_to_hash, taghashout, 16);
    RAND_bytes(value_to_hash+16, PTWIST_RESP_BYTES);
    value_to_hash[16+PTWIST_RESP_BYTES-1] &= PTWIST_RESP_MASK;

    while(1) {
	unsigned int firstbits;

	md_map_sh256(hashout, value_to_hash, puzzle_len);
#if PTWIST_PUZZLE_STRENGTH < 32
	/* This assumes that you're on an architecture that doesn't care
	 * about alignment, and is little endian. */
	firstbits = *(unsigned int*)hashout;
	if ((firstbits & PTWIST_PUZZLE_MASK) == 0) {
	    break;
	}
	/* Increment R and try again. */
	for(i=0;i<PTWIST_RESP_BYTES;++i) {
	    if (++value_to_hash[16+i]) break;
	}
	value_to_hash[16+PTWIST_RESP_BYTES-1] &= PTWIST_RESP_MASK;
#else
#error "Code assumes PTWIST_PUZZLE_STRENGTH < 32"
#endif
    }

	/*
	for(i=0;i<puzzle_len;++i) {
	    printf("%02x", value_to_hash[i]);
	    if ((i%4) == 3) printf(" ");
	}
	printf("\n");
	for(i=0;i<32;++i) {
	    printf("%02x", hashout[i]);
	    if ((i%4) == 3) printf(" ");
	}
	printf("\n");
	*/
    /* When we get here, we have solved the puzzle.  R is in
     * value_to_hash[16..16+PTWIST_RESP_BYTES-1], the hash output
     * hashout starts with PTWIST_PUZZLE_STRENGTH bits of 0s, and we'll
     * want to copy out H (the next PTWIST_HASH_SHOWBITS bits of the
     * hash output).  The final tag is [seckey*G]_x || R || H . */
    bn_new(Rbn);
    bn_new(Hbn);

    bn_read_bin(Rbn, value_to_hash+16, PTWIST_RESP_BYTES, BN_POS);
    hashout[PTWIST_HASH_TOTBYTES-1] &= PTWIST_HASH_MASK;
    bn_read_bin(Hbn, hashout, PTWIST_HASH_TOTBYTES, BN_POS);
    bn_lsh(Hbn, Hbn, PTWIST_RESP_BITS-PTWIST_PUZZLE_STRENGTH);
    bn_add(Hbn, Hbn, Rbn);
    len = PTWIST_TAG_BYTES-PTWIST_BYTES;
    bn_write_bin(tag+PTWIST_BYTES, &len, &sign, Hbn);
	/*
	for(i=0;i<PTWIST_TAG_BYTES;++i) {
	    printf("%02x", tag[i]);
	    if ((i%4) == 3) printf(" ");
	}
	printf("\n");
	*/

    bn_free(Rbn);
    bn_free(Hbn);
#elif PTWIST_HASH_SHOWBITS <= 128
    /* We're not using a client puzzle, so the tag is [seckey*G]_x || H
     * where H is the first PTWIST_HASH_SHOWBITS bits of the above hash
     * output.  The key generated is the last 128 bits of that output.
     * If there's no client puzzle, PTWIST_HASH_SHOWBITS must be a multiple
     * of 8. */
    memmove(tag+PTWIST_BYTES, taghashout, PTWIST_HASH_SHOWBITS/8);
#else
#error "No client puzzle used, but PWTIST_HASH_SHOWBITS > 128"
#endif

    memmove(key, taghashout+16, 16);
}

int main(int argc, char **argv)
{
    FILE *fp;
    int res;
    int i;
    byte tag[PTWIST_TAG_BYTES];
    int numtags = argc > 1 ? atoi(argv[1]) : 10;

    /* Create the generators */
    memset(maingen, 0, PTWIST_BYTES);
    maingen[0] = 2;
    memset(twistgen, 0, PTWIST_BYTES);

    /* Read the public keys */
    fp = fopen("pubkey", "rb");
    if (fp == NULL) {
	perror("fopen");
	exit(1);
    }
    res = fread(mainpub, PTWIST_BYTES, 1, fp);
    if (res < 1) {
	perror("fread");
	exit(1);
    }
    res = fread(twistpub, PTWIST_BYTES, 1, fp);
    if (res < 1) {
	perror("fread");
	exit(1);
    }
    fclose(fp);

    fp = fopen("tags", "wb");
    if (fp == NULL) {
	perror("fopen");
	exit(1);
    }
    for(i=0;i<numtags;++i) {
	byte key[16];
#if 0
	int j;
#endif
	gen_tag(tag, key, (const byte *)"context", 7);
	fwrite(tag, PTWIST_TAG_BYTES, 1, fp);
#if 0
	for(j=0;j<16;++j) {
	    printf("%02x", key[j]);
	}
	printf("\n");
#endif
    }
    fclose(fp);

    return 0;
}

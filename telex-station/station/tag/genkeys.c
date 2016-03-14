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

#include <openssl/rand.h>
#include <stdio.h>
#include <string.h>
#include "ptwist.h"

int main(int argc, char **argv)
{
    byte privkey[PTWIST_BYTES];
    byte maingen[PTWIST_BYTES];
    byte twistgen[PTWIST_BYTES];
    byte mainpub[PTWIST_BYTES];
    byte twistpub[PTWIST_BYTES];
    FILE *fp;

    /* Pick a random private key */
#if 1
    fp = fopen("/dev/random", "rb");
    if (fread(privkey, PTWIST_BYTES, 1, fp) != 1) {
        fprintf(stderr, "Error: did not read enough bytes\n");
        exit(1);
    }
    fclose(fp);
#else /* If above is too slow, switch to openssl PRNG: */ 
    RAND_bytes(privkey, PTWIST_BYTES);
#endif

    /* Load the generators of the main and twist curves */
    memset(maingen, 0, PTWIST_BYTES);
    maingen[0] = 2;
    memset(twistgen, 0, PTWIST_BYTES);

    /* Create the pubkeys */
    ptwist_pointmul(mainpub, maingen, privkey);
    ptwist_pointmul(twistpub, twistgen, privkey);

    /* Save the privkey and pubkeys */
    fp = fopen("privkey", "wb");
    fwrite(privkey, PTWIST_BYTES, 1, fp);
    fclose(fp);
    fp = fopen("pubkey", "wb");
    fwrite(mainpub, PTWIST_BYTES, 1, fp);
    fwrite(twistpub, PTWIST_BYTES, 1, fp);
    fclose(fp);

    return 0;
}

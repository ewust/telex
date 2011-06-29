#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <openssl/sha.h>
#include "ptwist.h"

/* Check the given tag with the given context and private key.  Return 0
   if the tag is properly formed, non-0 if not.  If the tag is correct,
   set key to the resulting secret key. */
static int check_tag(byte key[16], const byte privkey[PTWIST_BYTES],
	const byte tag[PTWIST_TAG_BYTES], const byte *context,
	size_t context_len)
{
    int ret = -1;
    byte sharedsec[PTWIST_BYTES+context_len];
    byte taghashout[32];
#if PTWIST_PUZZLE_STRENGTH > 0
    byte hashout[32];
    size_t puzzle_len = 16+PTWIST_RESP_BYTES;
    byte value_to_hash[puzzle_len];
    unsigned int firstbits;
    int firstpass = 0;
#endif

    /* Compute the shared secret privkey*TAG */
    ptwist_pointmul(sharedsec, tag, privkey);

    /* Create the hash tag keys */
    memmove(sharedsec+PTWIST_BYTES, context, context_len);
    SHA256(sharedsec, PTWIST_BYTES+context_len, taghashout);

#if PTWIST_PUZZLE_STRENGTH > 0
    /* Construct the proposed solution to the puzzle */
    memmove(value_to_hash, taghashout, 16);
    memmove(value_to_hash+16, tag+PTWIST_BYTES, PTWIST_RESP_BYTES);
    value_to_hash[16+PTWIST_RESP_BYTES-1] &= PTWIST_RESP_MASK;

    /* Hash the proposed solution and see if it is correct; that is, the
     * hash should start with PTWIST_PUZZLE_STRENGTH bits of 0s,
     * followed by the last PTWIST_HASH_SHOWBITS of the tag. */
    md_map_sh256(hashout, value_to_hash, puzzle_len);
#if PTWIST_PUZZLE_STRENGTH < 32
    /* This assumes that you're on an architecture that doesn't care
     * about alignment, and is little endian. */
    firstbits = *(unsigned int*)hashout;
    if ((firstbits & PTWIST_PUZZLE_MASK) == 0) {
	firstpass = 1;
    }
#else
#error "Code assumes PTWIST_PUZZLE_STRENGTH < 32"
#endif
    if (firstpass) {
	bn_t Hbn, Tbn;
	bn_new(Hbn);
	bn_new(Tbn);
	hashout[PTWIST_HASH_TOTBYTES-1] &= PTWIST_HASH_MASK;
	bn_read_bin(Hbn, hashout, PTWIST_HASH_TOTBYTES, BN_POS);
	bn_rsh(Hbn, Hbn, PTWIST_PUZZLE_STRENGTH);
	bn_read_bin(Tbn, tag+PTWIST_BYTES, PTWIST_TAG_BYTES-PTWIST_BYTES,
		    BN_POS);
	bn_rsh(Tbn, Tbn, PTWIST_RESP_BITS);

	ret = (bn_cmp(Tbn,Hbn) != CMP_EQ);

	bn_free(Hbn);
	bn_free(Tbn);
    }
#else
    /* We're not using a client puzzle, so just check that the first
     * PTWIST_HASH_SHOWBITS bits of the above hash fill out the rest
     * of the tag.  If there's no puzzle, PTWIST_HASH_SHOWBITS must be
     * a multiple of 8. */
    ret = (memcmp(tag+PTWIST_BYTES, taghashout, PTWIST_HASH_SHOWBITS/8) != 0);
#endif
    if (ret == 0) {
	memmove(key, taghashout+16, 16);
    }
    return ret;
}

int main(int argc, char **argv)
{
    FILE *fp;
    int res;
    byte privkey[PTWIST_BYTES];
    byte tag[PTWIST_TAG_BYTES];
    int tottagged = 0;
    int totuntagged = 0;
    const char *tagsfile = argc > 1 ? argv[1] : "tags";

    /* Load the private key */
    fp = fopen("privkey", "rb");
    if (fp == NULL) {
	perror("fopen");
	exit(1);
    }
    res = fread(privkey, PTWIST_BYTES, 1, fp);
    if (res < 1) {
	perror("fread");
	exit(1);
    }
    fclose(fp);

    /* Validate the tags in the tags file */
    fp = fopen(tagsfile, "rb");
    if (fp == NULL) {
	perror("fopen");
	exit(1);
    }
    while (fread(tag, PTWIST_TAG_BYTES, 1, fp) == 1) {
	byte key[16];
	int res = check_tag(key, privkey, tag, (const byte *)"context", 7);
	tottagged += (!res);
	totuntagged += (!(!res));
#if 0
	if (res) {
	    printf("Untagged\n");
	} else {
	    int j;
	    for(j=0;j<16;++j) {
		printf("%02x", key[j]);
	    }
	    printf("\n");
	}
#endif
    }
    fclose(fp);

    printf("%d tagged, %d untagged\n", tottagged, totuntagged);

    return 0;
}

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

#ifndef __PTWIST_H__
#define __PTWIST_H__

#ifdef __cplusplus
extern "C" {
#endif

#define PTWIST_BITS 168  /* must be a multiple of 8 */
#define PTWIST_BYTES (PTWIST_BITS/8)

#define PTWIST_TAG_BITS 224  /* must be a multiple of 8 */
#define PTWIST_TAG_BYTES (PTWIST_TAG_BITS/8)

#define PTWIST_PUZZLE_STRENGTH 0 /*21*/  /* set to 0 to disable client puzzle */
#define PTWIST_PUZZLE_MASK ((1<<PTWIST_PUZZLE_STRENGTH)-1)

#if PTWIST_PUZZLE_STRENGTH == 0
#define PTWIST_RESP_BITS 0
#else
#define PTWIST_RESP_BITS (PTWIST_PUZZLE_STRENGTH+6)
#endif

#define PTWIST_RESP_BYTES ((PTWIST_RESP_BITS+7)/8)
#define PTWIST_RESP_MASK ((1<<(((PTWIST_RESP_BITS&7)==0)?8:(PTWIST_RESP_BITS&7)))-1)

#define PTWIST_HASH_SHOWBITS (PTWIST_TAG_BITS-PTWIST_BITS-PTWIST_RESP_BITS)
#define PTWIST_HASH_TOTBITS (PTWIST_HASH_SHOWBITS+PTWIST_PUZZLE_STRENGTH)
#define PTWIST_HASH_TOTBYTES ((PTWIST_HASH_TOTBITS+7)/8)
#define PTWIST_HASH_MASK ((1<<(((PTWIST_HASH_TOTBITS&7)==0)?8:(PTWIST_HASH_TOTBITS&7)))-1)

typedef unsigned char byte;

/* Figure out whether there's a point with x-coordinate x on the main
 * curve.  If not, then there's one on the twist curve.  (There are
 * actually two, which are negatives of each other; that doesn't
 * matter.)  Multiply that point by seckey and set out to the
 * x-coordinate of the result. */
void ptwist_pointmul(byte out[PTWIST_BYTES], const byte x[PTWIST_BYTES],
	const byte seckey[PTWIST_BYTES]);

#ifdef __cplusplus
}
#endif

#endif

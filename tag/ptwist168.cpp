#include "ptwist.h"

/* ptwist168.c by Ian Goldberg. Based on: */

/* crypto/ec/ecp_nistp224.c */
/*
 * Written by Emilia Kasper (Google) for the OpenSSL project.
 */
/* ====================================================================
 * Copyright (c) 2000-2010 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.OpenSSL.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    licensing@OpenSSL.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.OpenSSL.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com).
 *
 */

/*
 * A 64-bit implementation of the NIST P-224 elliptic curve point multiplication
 *
 * Inspired by Daniel J. Bernstein's public domain nistp224 implementation
 * and Adam Langley's public domain 64-bit C implementation of curve25519
 */
#include <stdint.h>
#include <string.h>

#if  defined(__GNUC__) && defined(__LP64__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
  /* even with gcc, the typedef won't work for 32-bit platforms */
  typedef __uint128_t uint128_t; /* nonstandard; implemented by gcc on 64-bit platforms */
#  define Uint128Low64(x) ((uint64_t)(x))
#else
  #include "int128.h"
  typedef uint128 uint128_t;
#endif

typedef uint8_t u8;


/******************************************************************************/
/*		    INTERNAL REPRESENTATION OF FIELD ELEMENTS
 *
 * Field elements are represented as a_0 + 2^56*a_1 + 2^112*a_2
 * where each slice a_i is a 64-bit word, i.e., a field element is an fslice
 * array a with 3 elements, where a[i] = a_i.
 * Outputs from multiplications are represented as unreduced polynomials
 * b_0 + 2^56*b_1 + 2^112*b_2 + 2^168*b_3 + 2^224*b_4
 * where each b_i is a 128-bit word. We ensure that inputs to each field
 * multiplication satisfy a_i < 2^60, so outputs satisfy b_i < 4*2^60*2^60,
 * and fit into a 128-bit word without overflow. The coefficients are then
 * again partially reduced to a_i < 2^57. We only reduce to the unique minimal
 * representation at the end of the computation.
 *
 */

typedef uint64_t fslice;

typedef fslice coord[3];
typedef coord point[3];


#include <stdio.h>
#include <stdlib.h>

static void dump_coord(const char *label, const coord c)
{
    if (label) fprintf(stderr, "%s: ", label);
    printf("%016lx %016lx %016lx\n", c[2], c[1], c[0]);
}

static void dump_point(const char *label, point p)
{
    if (label) fprintf(stderr, "%s:\n", label);
    dump_coord(" x", p[0]);
    dump_coord(" y", p[1]);
    dump_coord(" z", p[2]);
}

/* Field element represented as a byte arrary.
 * 21*8 = 168 bits is also the group order size for the elliptic curve.  */
typedef u8 felem_bytearray[21];

static const felem_bytearray ptwist168_curve_params[5] = {
	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,    /* p */
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,
	 0xFF},
	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,    /* a */
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFF,0xFF,0xFF,0xFE,
	 0xFC},
	{0x4E,0x35,0x5E,0x95,0xCA,0xFE,0xDD,0x48,0x6E,0xBC,    /* b */
	 0x69,0xBA,0xD3,0x16,0x46,0xD3,0x20,0xE0,0x1D,0xC7,
	 0xD6},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,    /* x */
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x02},
	{0xEA,0x67,0x47,0xB7,0x5A,0xF8,0xC7,0xF9,0x3C,0x1F,    /* y */
	 0x5E,0x6D,0x32,0x0F,0x88,0xB9,0xBE,0x15,0x66,0xD2,
	 0xF2}
};

/* Helper functions to convert field elements to/from internal representation */
static void bin21_to_felem(fslice out[3], const u8 in[21])
	{
	out[0] = *((const uint64_t *)(in)) & 0x00ffffffffffffff;
	out[1] = (*((const uint64_t *)(in+7))) & 0x00ffffffffffffff;
	out[2] = (*((const uint64_t *)(in+14))) & 0x00ffffffffffffff;
	}

static void felem_to_bin21(u8 out[21], const fslice in[3])
	{
	unsigned i;
	for (i = 0; i < 7; ++i)
		{
		out[i]	  = in[0]>>(8*i);
		out[i+7]  = in[1]>>(8*i);
		out[i+14] = in[2]>>(8*i);
		}
	}

#if 0
/* To preserve endianness when using BN_bn2bin and BN_bin2bn */
static void flip_endian(u8 *out, const u8 *in, unsigned len)
	{
	unsigned i;
	for (i = 0; i < len; ++i)
		out[i] = in[len-1-i];
	}
#endif

/******************************************************************************/
/*				FIELD OPERATIONS
 *
 * Field operations, using the internal representation of field elements.
 * NB! These operations are specific to our point multiplication and cannot be
 * expected to be correct in general - e.g., multiplication with a large scalar
 * will cause an overflow.
 *
 */

/* Sum two field elements: out += in */
static void felem_sum64(fslice out[3], const fslice in[3])
	{
	out[0] += in[0];
	out[1] += in[1];
	out[2] += in[2];
	}

/* Subtract field elements: out -= in */
/* Assumes in[i] < 2^57 */
static void felem_diff64(fslice out[3], const fslice in[3])
	{
	/* a = 3*2^56 - 3 */
	/* b = 3*2^56 - 3*257 */
	static const uint64_t a = (((uint64_t) 3) << 56) - ((uint64_t) 3);
	static const uint64_t b = (((uint64_t) 3) << 56) - ((uint64_t) 771);

	/* Add 0 mod 2^168-2^8-1 to ensure out > in at each element */
	/* a*2^112 + a*2^56 + b = 3*p */
	out[0] += b;
	out[1] += a;
	out[2] += a;

	out[0] -= in[0];
	out[1] -= in[1];
	out[2] -= in[2];
	}

/* Subtract in unreduced 128-bit mode: out128 -= in128 */
/* Assumes in[i] < 2^119 */
static void felem_diff128(uint128_t out[5], const uint128_t in[5])
	{
	/* a = 3*2^118 - 192
	   b = 3*2^118 - 49536
	   c = 3*2^118
	   d = 3*2^118 - 12681408

	   a*2^224 + a*2^168 + b*2^112 + c*2^56 + d
	    = (3*2^174 + 3*2^118 + 49344)*p
	*/
	static const uint128_t a = (((uint128_t)3) << 118) - ((uint128_t) 192);
	static const uint128_t b = (((uint128_t)3) << 118) - ((uint128_t) 49536);
	static const uint128_t c = (((uint128_t)3) << 118);
	static const uint128_t d = (((uint128_t)3) << 118) - ((uint128_t) 12681408);;

	/* Add 0 mod 2^168-2^8-1 to ensure out > in */
	out[0] += d;
	out[1] += c;
	out[2] += b;
	out[3] += a;
	out[4] += a;

	out[0] -= in[0];
	out[1] -= in[1];
	out[2] -= in[2];
	out[3] -= in[3];
	out[4] -= in[4];
	}

/* Subtract in mixed mode: out128 -= in64 */
/* in[i] < 2^63 */
static void felem_diff_128_64(uint128_t out[5], const fslice in[3])
	{
	/* a = 3*2^62 - 192
	   b = 3*2^62 - 49344
	   a*2^112 + a*2^56 + b = 192*p
	*/
	static const uint128_t a = (((uint128_t) 3) << 62) - ((uint128_t) 192);
	static const uint128_t b = (((uint128_t) 3) << 62) - ((uint128_t) 49344);

	/* Add 0 mod 2^168-2^8-1 to ensure out > in */
	out[0] += b;
	out[1] += a;
	out[2] += a;

	out[0] -= in[0];
	out[1] -= in[1];
	out[2] -= in[2];
	}

/* Multiply a field element by a scalar: out64 = out64 * scalar
 * The scalars we actually use are small, so results fit without overflow */
static void felem_scalar64(fslice out[3], const fslice scalar)
	{
	out[0] *= scalar;
	out[1] *= scalar;
	out[2] *= scalar;
	}

/* Multiply an unreduced field element by a scalar: out128 = out128 * scalar
 * The scalars we actually use are small, so results fit without overflow */
static void felem_scalar128(uint128_t out[5], const uint128_t scalar)
	{
	out[0] *= scalar;
	out[1] *= scalar;
	out[2] *= scalar;
	out[3] *= scalar;
	out[4] *= scalar;
	}

/* Square a field element: out = in^2 */
static void felem_square(uint128_t out[5], const fslice in[3])
	{
	out[0] = ((uint128_t) in[0]) * in[0];
	out[1] = ((uint128_t) in[0]) * in[1] * 2;
	out[2] = ((uint128_t) in[0]) * in[2] * 2 + ((uint128_t) in[1]) * in[1];
	out[3] = ((uint128_t) in[1]) * in[2] * 2;
	out[4] = ((uint128_t) in[2]) * in[2];
	}

/* Multiply two field elements: out = in1 * in2 */
static void felem_mul(uint128_t out[5], const fslice in1[3], const fslice in2[3])
	{
	out[0] = ((uint128_t) in1[0]) * in2[0];
	out[1] = ((uint128_t) in1[0]) * in2[1] + ((uint128_t) in1[1]) * in2[0];
	out[2] = ((uint128_t) in1[0]) * in2[2] + ((uint128_t) in1[1]) * in2[1] +
		((uint128_t) in1[2]) * in2[0];
	out[3] = ((uint128_t) in1[1]) * in2[2] +
		((uint128_t) in1[2]) * in2[1];
	out[4] = ((uint128_t) in1[2]) * in2[2];
	}

#define M257(x) (((x)<<8)+(x))

/* Reduce 128-bit coefficients to 64-bit coefficients. Requires in[i] < 2^126,
 * ensures out[0] < 2^56, out[1] < 2^56, out[2] < 2^57 */
static void felem_reduce(fslice out[3], const uint128_t in[5])
	{
	static const uint128_t two56m1 = (((uint128_t) 1)<<56) -
		((uint128_t)1);
	uint128_t output[3];

	output[0] = in[0];  /* < 2^126 */
	output[1] = in[1];  /* < 2^126 */
	output[2] = in[2];  /* < 2^126 */

	/* Eliminate in[3], in[4] */
	output[2] += M257(in[4] >> 56);       /* < 2^126 + 2^79 */
	output[1] += M257(in[4] & two56m1);   /* < 2^126 + 2^65 */

	output[1] += M257(in[3] >> 56);       /* < 2^126 + 2^65 + 2^79 */
	output[0] += M257(in[3] & two56m1);   /* < 2^126 + 2^65 */

	/* Eliminate the top part of output[2] */
	output[0] += M257(output[2] >> 56);   /* < 2^126 + 2^65 + 2^79 */
	output[2] &= two56m1;                 /* < 2^56 */

	/* Carry 0 -> 1 -> 2 */
	output[1] += output[0] >> 56;         /* < 2^126 + 2^71 */
	output[0] &= two56m1;                 /* < 2^56 */

	output[2] += output[1] >> 56;         /* < 2^71 */
	output[1] &= two56m1;                 /* < 2^56 */

	/* Eliminate the top part of output[2] */
	output[0] += M257(output[2] >> 56);   /* < 2^57 */
	output[2] &= two56m1;                 /* < 2^56 */

	/* Carry 0 -> 1 -> 2 */
	output[1] += output[0] >> 56;         /* <= 2^56 */
	out[0] = Uint128Low64(output[0] & two56m1);         /* < 2^56 */

	out[2] = Uint128Low64(output[2] + (output[1] >> 56));  /* <= 2^56 */
	out[1] = Uint128Low64(output[1] & two56m1);         /* < 2^56 */

	}

/* Reduce to unique minimal representation */
static void felem_contract(fslice out[3], const fslice in[3])
	{
	static const uint64_t two56m1 = (((uint64_t) 1)<<56) -
		((uint64_t)1);
	static const uint64_t two56m257 = (((uint64_t) 1)<<56) -
		((uint64_t)257);
	uint64_t a;

	/* in[0] < 2^56, in[1] < 2^56, in[2] <= 2^56 */
	/* so in < 2*p for sure */

	/* Eliminate the top part of in[2] */
	out[0] = in[0] + M257(in[2] >> 56);   /* < 2^57 */
	out[2] = in[2] & two56m1;             /* < 2^56, but if out[0] >= 2^56
	                                         then out[2] now = 0 */

	/* Carry 0 -> 1 -> 2 */
	out[1] = in[1] + (out[0] >> 56);      /* < 2^56 + 2, but if
	                                         out[1] >= 2^56 then
						 out[2] = 0 */
	out[0] &= two56m1;                    /* < 2^56 */

	out[2] += out[1] >> 56;               /* < 2^56 due to the above */
	out[1] &= two56m1;                    /* < 2^56 */

	/* Now out < 2^168, but it could still be > p */
	a = ((out[2] == two56m1) & (out[1] == two56m1) & (out[0] >= two56m257));
	out[2] -= two56m1*a;
	out[1] -= two56m1*a;
	out[0] -= two56m257*a;
	}

/* Negate a field element: out = -in */
/* Assumes in[i] < 2^57 */
static void felem_neg(fslice out[3], const fslice in[3])
	{
	/* a = 3*2^56 - 3 */
	/* b = 3*2^56 - 3*257 */
	static const uint64_t a = (((uint64_t) 3) << 56) - ((uint64_t) 3);
	static const uint64_t b = (((uint64_t) 3) << 56) - ((uint64_t) 771);
	static const uint64_t two56m1 = (((uint64_t) 1) << 56) - ((uint64_t) 1);
	fslice tmp[3];

	/* Add 0 mod 2^168-2^8-1 to ensure out > in at each element */
	/* a*2^112 + a*2^56 + b = 3*p */
	tmp[0] = b - in[0];
	tmp[1] = a - in[1];
	tmp[2] = a - in[2];

	/* Carry 0 -> 1 -> 2 */
	tmp[1] += tmp[0] >> 56;
	tmp[0] &= two56m1;                 /* < 2^56 */

	tmp[2] += tmp[1] >> 56;         /* < 2^71 */
	tmp[1] &= two56m1;                 /* < 2^56 */

	felem_contract(out, tmp);

	}

/* Zero-check: returns 1 if input is 0, and 0 otherwise.
 * We know that field elements are reduced to in < 2^169,
 * so we only need to check three cases: 0, 2^168 - 2^8 - 1,
 * and 2^169 - 2^9 - 2 */
static fslice felem_is_zero(const fslice in[3])
	{
	fslice zero, two168m8m1, two169m9m2;
	static const uint64_t two56m1 = (((uint64_t) 1)<<56) -
		((uint64_t)1);
	static const uint64_t two56m257 = (((uint64_t) 1)<<56) -
		((uint64_t)257);
	static const uint64_t two57m1 = (((uint64_t) 1)<<57) -
		((uint64_t)1);
	static const uint64_t two56m514 = (((uint64_t) 1)<<56) -
		((uint64_t)514);

	zero = (in[0] == 0) & (in[1] == 0) & (in[2] == 0);
	two168m8m1 = (in[2] == two56m1) & (in[1] == two56m1) &
			(in[0] == two56m257);
	two169m9m2 = (in[2] == two57m1) & (in[1] == two56m1) &
			(in[0] == two56m514);

	return (zero | two168m8m1 | two169m9m2);
	}

/* Invert a field element */
static void felem_inv(fslice out[3], const fslice in[3])
	{
	fslice ftmp[3], ftmp2[3], ftmp3[3], ftmp4[3];
	uint128_t tmp[5];
	unsigned i;

	felem_square(tmp, in); felem_reduce(ftmp, tmp);		/* 2 */
	felem_mul(tmp, in, ftmp); felem_reduce(ftmp, tmp);	/* 2^2 - 1 */
								/* = ftmp */

	felem_square(tmp, ftmp); felem_reduce(ftmp2, tmp);	/* 2^3 - 2 */
	felem_square(tmp, ftmp2); felem_reduce(ftmp2, tmp);	/* 2^4 - 2^2 */
	felem_mul(tmp, ftmp2, ftmp); felem_reduce(ftmp2, tmp);	/* 2^4 - 1 */
	felem_square(tmp, ftmp2); felem_reduce(ftmp2, tmp);	/* 2^5 - 2 */
	felem_square(tmp, ftmp2); felem_reduce(ftmp2, tmp);	/* 2^6 - 2^2 */
	felem_mul(tmp, ftmp2, ftmp); felem_reduce(ftmp, tmp);	/* 2^6 - 1 */
								/* = ftmp */

	felem_square(tmp, ftmp); felem_reduce(ftmp2, tmp);	/* 2^7 - 2 */
	for (i = 0; i < 5; ++i)					/* 2^12 - 2^6 */
		{
		felem_square(tmp, ftmp2); felem_reduce(ftmp2, tmp);
		}
	felem_mul(tmp, ftmp, ftmp2); felem_reduce(ftmp3, tmp);	/* 2^12 - 1 */
								/* = ftmp3 */

	felem_square(tmp, ftmp3); felem_reduce(ftmp2, tmp);	/* 2^13 - 2 */
	for (i = 0; i < 11; ++i)				/* 2^24 - 2^12 */
		{
		felem_square(tmp, ftmp2); felem_reduce(ftmp2, tmp);
		}
	felem_mul(tmp, ftmp2, ftmp3); felem_reduce(ftmp3, tmp);	/* 2^24 - 1 */
								/* = ftmp3 */
	felem_square(tmp, ftmp3); felem_reduce(ftmp2, tmp);	/* 2^25 - 2 */
	for (i = 0; i < 23; ++i)				/* 2^48 - 2^24 */
		{
		felem_square(tmp, ftmp2); felem_reduce(ftmp2, tmp);
		}
	felem_mul(tmp, ftmp2, ftmp3); felem_reduce(ftmp4, tmp);	/* 2^48 - 1 */
								/* = ftmp4 */
	felem_square(tmp, ftmp4); felem_reduce(ftmp2, tmp);	/* 2^49 - 2 */
	for (i = 0; i < 23; ++i)				/* 2^72 - 2^24 */
		{
		felem_square(tmp, ftmp2); felem_reduce(ftmp2, tmp);
		}
	felem_mul(tmp, ftmp2, ftmp3); felem_reduce(ftmp4, tmp);	/* 2^72 - 1 */
								/* = ftmp4 */

	felem_square(tmp, ftmp4); felem_reduce(ftmp2, tmp);	/* 2^73 - 2 */
	for (i = 0; i < 5; ++i)					/* 2^78 - 2^6 */
		{
		felem_square(tmp, ftmp2); felem_reduce(ftmp2, tmp);
		}
	felem_mul(tmp, ftmp, ftmp2); felem_reduce(ftmp2, tmp);	/* 2^78 - 1 */
	felem_square(tmp, ftmp2); felem_reduce(ftmp2, tmp);	/* 2^79 - 2 */
	felem_mul(tmp, in, ftmp2); felem_reduce(ftmp4, tmp);	/* 2^79 - 1 */
								/* = ftmp4 */
	felem_square(tmp, ftmp4); felem_reduce(ftmp2, tmp);	/* 2^80 - 2 */
	for (i = 0; i < 78; ++i)				/* 2^158 - 2^79 */
		{
		felem_square(tmp, ftmp2); felem_reduce(ftmp2, tmp);
		}
	felem_mul(tmp, ftmp4, ftmp2); felem_reduce(ftmp2, tmp); /* 2^158 - 1 */
	felem_square(tmp, ftmp2); felem_reduce(ftmp2, tmp);	/* 2^159 - 2 */
	felem_mul(tmp, in, ftmp2); felem_reduce(ftmp2, tmp);	/* 2^159 - 1 */
	for (i = 0; i < 7; ++i)					/* 2^166 - 2^7 */
		{
		felem_square(tmp, ftmp2); felem_reduce(ftmp2, tmp);
		}
	felem_mul(tmp, ftmp, ftmp2); felem_reduce(ftmp2, tmp);	/* 2^166 - 2^6 - 1 */
	felem_square(tmp, ftmp2); felem_reduce(ftmp2, tmp);	/* 2^167 - 2^7 - 2 */
	felem_square(tmp, ftmp2); felem_reduce(ftmp2, tmp);	/* 2^168 - 2^8 - 4 */
	felem_mul(tmp, in, ftmp2); felem_reduce(out, tmp);	/* 2^168 - 2^8 - 3 */
								/* = out */
	}

/* Take the square root of a field element */
static void felem_sqrt(fslice out[3], const fslice in[3])
	{
	fslice ftmp[3], ftmp2[3];
	uint128_t tmp[5];
	unsigned i;

	felem_square(tmp, in); felem_reduce(ftmp, tmp);		/* 2 */
	felem_mul(tmp, in, ftmp); felem_reduce(ftmp, tmp);	/* 2^2 - 1 */
								/* = ftmp */

	felem_square(tmp, ftmp); felem_reduce(ftmp2, tmp);	/* 2^3 - 2 */
	felem_square(tmp, ftmp2); felem_reduce(ftmp2, tmp);	/* 2^4 - 2^2 */
	felem_mul(tmp, ftmp2, ftmp); felem_reduce(ftmp2, tmp);	/* 2^4 - 1 */
	felem_square(tmp, ftmp2); felem_reduce(ftmp2, tmp);	/* 2^5 - 2 */
	felem_mul(tmp, ftmp2, in); felem_reduce(ftmp, tmp);	/* 2^5 - 1 */
								/* = ftmp */

	felem_square(tmp, ftmp); felem_reduce(ftmp2, tmp);	/* 2^6 - 2 */
	for (i = 0; i < 4; ++i)					/* 2^10 - 2^5 */
		{
		felem_square(tmp, ftmp2); felem_reduce(ftmp2, tmp);
		}
	felem_mul(tmp, ftmp, ftmp2); felem_reduce(ftmp, tmp);	/* 2^10 - 1 */
								/* = ftmp */

	felem_square(tmp, ftmp); felem_reduce(ftmp2, tmp);	/* 2^11 - 2 */
	for (i = 0; i < 9; ++i)					/* 2^20 - 2^10 */
		{
		felem_square(tmp, ftmp2); felem_reduce(ftmp2, tmp);
		}
	felem_mul(tmp, ftmp2, ftmp); felem_reduce(ftmp, tmp);	/* 2^20 - 1 */
								/* = ftmp */
	felem_square(tmp, ftmp); felem_reduce(ftmp2, tmp);	/* 2^21 - 2 */
	for (i = 0; i < 19; ++i)				/* 2^40 - 2^20 */
		{
		felem_square(tmp, ftmp2); felem_reduce(ftmp2, tmp);
		}
	felem_mul(tmp, ftmp2, ftmp); felem_reduce(ftmp, tmp);	/* 2^40 - 1 */
								/* = ftmp */
	felem_square(tmp, ftmp); felem_reduce(ftmp2, tmp);	/* 2^41 - 2 */
	for (i = 0; i < 39; ++i)				/* 2^80 - 2^40 */
		{
		felem_square(tmp, ftmp2); felem_reduce(ftmp2, tmp);
		}
	felem_mul(tmp, ftmp2, ftmp); felem_reduce(ftmp, tmp);	/* 2^80 - 1 */
								/* = ftmp */

	felem_square(tmp, ftmp); felem_reduce(ftmp2, tmp);	/* 2^81 - 2 */
	for (i = 0; i < 79; ++i)				/* 2^160 - 2^80 */
		{
		felem_square(tmp, ftmp2); felem_reduce(ftmp2, tmp);
		}
	felem_mul(tmp, ftmp, ftmp2); felem_reduce(ftmp2, tmp);	/* 2^160 - 1 */
	for (i = 0; i < 5; ++i)					/* 2^165 - 2^5 */
		{
		felem_square(tmp, ftmp2); felem_reduce(ftmp2, tmp);
		}
	felem_square(tmp, ftmp2); felem_reduce(out, tmp);	/* 2^166 - 2^6 */
								/* = out */
	}

/* Copy in constant time:
 * if icopy == 1, copy in to out,
 * if icopy == 0, copy out to itself. */
static void
copy_conditional(fslice *out, const fslice *in, unsigned len, fslice icopy)
	{
	unsigned i;
	/* icopy is a (64-bit) 0 or 1, so copy is either all-zero or all-one */
	const fslice copy = -icopy;
	for (i = 0; i < len; ++i)
		{
		const fslice tmp = copy & (in[i] ^ out[i]);
		out[i] ^= tmp;
		}
	}

#if 0
/* Copy in constant time:
 * if isel == 1, copy in2 to out,
 * if isel == 0, copy in1 to out. */
static void select_conditional(fslice *out, const fslice *in1, const fslice *in2,
	unsigned len, fslice isel)
	{
	unsigned i;
	/* isel is a (64-bit) 0 or 1, so sel is either all-zero or all-one */
	const fslice sel = -isel;
	for (i = 0; i < len; ++i)
		{
		const fslice tmp = sel & (in1[i] ^ in2[i]);
		out[i] = in1[i] ^ tmp;
		}
}
#endif

/******************************************************************************/
/*			 ELLIPTIC CURVE POINT OPERATIONS
 *
 * Points are represented in Jacobian projective coordinates:
 * (X, Y, Z) corresponds to the affine point (X/Z^2, Y/Z^3),
 * or to the point at infinity if Z == 0.
 *
 */

/* Double an elliptic curve point:
 * (X', Y', Z') = 2 * (X, Y, Z), where
 * X' = (3 * (X - Z^2) * (X + Z^2))^2 - 8 * X * Y^2
 * Y' = 3 * (X - Z^2) * (X + Z^2) * (4 * X * Y^2 - X') - 8 * Y^2
 * Z' = (Y + Z)^2 - Y^2 - Z^2 = 2 * Y * Z
 * Outputs can equal corresponding inputs, i.e., x_out == x_in is allowed,
 * while x_out == y_in is not (maybe this works, but it's not tested). */
static void
point_double(fslice x_out[3], fslice y_out[3], fslice z_out[3],
	     const fslice x_in[3], const fslice y_in[3], const fslice z_in[3])
	{
	uint128_t tmp[5], tmp2[5];
	fslice delta[3];
	fslice gamma[3];
	fslice beta[3];
	fslice alpha[3];
	fslice ftmp[3], ftmp2[3];
	memcpy(ftmp, x_in, 3 * sizeof(fslice));
	memcpy(ftmp2, x_in, 3 * sizeof(fslice));

	/* delta = z^2 */
	felem_square(tmp, z_in);
	felem_reduce(delta, tmp);

	/* gamma = y^2 */
	felem_square(tmp, y_in);
	felem_reduce(gamma, tmp);

	/* beta = x*gamma */
	felem_mul(tmp, x_in, gamma);
	felem_reduce(beta, tmp);

	/* alpha = 3*(x-delta)*(x+delta) */
	felem_diff64(ftmp, delta);
	/* ftmp[i] < 2^57 + 2^58 + 2 < 2^59 */
	felem_sum64(ftmp2, delta);
	/* ftmp2[i] < 2^57 + 2^57 = 2^58 */
	felem_scalar64(ftmp2, 3);
	/* ftmp2[i] < 3 * 2^58 < 2^60 */
	felem_mul(tmp, ftmp, ftmp2);
	/* tmp[i] < 2^60 * 2^59 * 4 = 2^121 */
	felem_reduce(alpha, tmp);

	/* x' = alpha^2 - 8*beta */
	felem_square(tmp, alpha);
	/* tmp[i] < 4 * 2^57 * 2^57 = 2^116 */
	memcpy(ftmp, beta, 3 * sizeof(fslice));
	felem_scalar64(ftmp, 8);
	/* ftmp[i] < 8 * 2^57 = 2^60 */
	felem_diff_128_64(tmp, ftmp);
	/* tmp[i] < 2^116 + 2^64 + 8 < 2^117 */
	felem_reduce(x_out, tmp);

	/* z' = (y + z)^2 - gamma - delta */
	felem_sum64(delta, gamma);
	/* delta[i] < 2^57 + 2^57 = 2^58 */
	memcpy(ftmp, y_in, 3 * sizeof(fslice));
	felem_sum64(ftmp, z_in);
	/* ftmp[i] < 2^57 + 2^57 = 2^58 */
	felem_square(tmp, ftmp);
	/* tmp[i] < 4 * 2^58 * 2^58 = 2^118 */
	felem_diff_128_64(tmp, delta);
	/* tmp[i] < 2^118 + 2^64 + 8 < 2^119 */
	felem_reduce(z_out, tmp);

	/* y' = alpha*(4*beta - x') - 8*gamma^2 */
	felem_scalar64(beta, 4);
	/* beta[i] < 4 * 2^57 = 2^59 */
	felem_diff64(beta, x_out);
	/* beta[i] < 2^59 + 2^58 + 2 < 2^60 */
	felem_mul(tmp, alpha, beta);
	/* tmp[i] < 4 * 2^57 * 2^60 = 2^119 */
	felem_square(tmp2, gamma);
	/* tmp2[i] < 4 * 2^57 * 2^57 = 2^116 */
	felem_scalar128(tmp2, 8);
	/* tmp2[i] < 8 * 2^116 = 2^119 */
	felem_diff128(tmp, tmp2);
	/* tmp[i] < 2^119 + 2^120 < 2^121 */
	felem_reduce(y_out, tmp);
	}

/* Add two elliptic curve points:
 * (X_1, Y_1, Z_1) + (X_2, Y_2, Z_2) = (X_3, Y_3, Z_3), where
 * X_3 = (Z_1^3 * Y_2 - Z_2^3 * Y_1)^2 - (Z_1^2 * X_2 - Z_2^2 * X_1)^3 -
 * 2 * Z_2^2 * X_1 * (Z_1^2 * X_2 - Z_2^2 * X_1)^2
 * Y_3 = (Z_1^3 * Y_2 - Z_2^3 * Y_1) * (Z_2^2 * X_1 * (Z_1^2 * X_2 - Z_2^2 * X_1)^2 - X_3) -
 *        Z_2^3 * Y_1 * (Z_1^2 * X_2 - Z_2^2 * X_1)^3
 * Z_3 = (Z_1^2 * X_2 - Z_2^2 * X_1) * (Z_1 * Z_2) */

/* This function is not entirely constant-time:
 * it includes a branch for checking whether the two input points are equal,
 * (while not equal to the point at infinity).
 * This case never happens during single point multiplication,
 * so there is no timing leak for ECDH or ECDSA signing. */
static void point_add(fslice x3[3], fslice y3[3], fslice z3[3],
	const fslice x1[3], const fslice y1[3], const fslice z1[3],
	const fslice x2[3], const fslice y2[3], const fslice z2[3])
	{
	fslice ftmp[3], ftmp2[3], ftmp3[3], ftmp4[3], ftmp5[3];
	uint128_t tmp[5], tmp2[5];
	fslice z1_is_zero, z2_is_zero, x_equal, y_equal;

	/* ftmp = z1^2 */
	felem_square(tmp, z1);
	felem_reduce(ftmp, tmp);

	/* ftmp2 = z2^2 */
	felem_square(tmp, z2);
	felem_reduce(ftmp2, tmp);

	/* ftmp3 = z1^3 */
	felem_mul(tmp, ftmp, z1);
	felem_reduce(ftmp3, tmp);

	/* ftmp4 = z2^3 */
	felem_mul(tmp, ftmp2, z2);
	felem_reduce(ftmp4, tmp);

	/* ftmp3 = z1^3*y2 */
	felem_mul(tmp, ftmp3, y2);
	/* tmp[i] < 4 * 2^57 * 2^57 = 2^116 */

	/* ftmp4 = z2^3*y1 */
	felem_mul(tmp2, ftmp4, y1);
	felem_reduce(ftmp4, tmp2);

	/* ftmp3 = z1^3*y2 - z2^3*y1 */
	felem_diff_128_64(tmp, ftmp4);
	/* tmp[i] < 2^116 + 2^64 + 8 < 2^117 */
	felem_reduce(ftmp3, tmp);

	/* ftmp = z1^2*x2 */
	felem_mul(tmp, ftmp, x2);
	/* tmp[i] < 4 * 2^57 * 2^57 = 2^116 */

	/* ftmp2 =z2^2*x1 */
	felem_mul(tmp2, ftmp2, x1);
	felem_reduce(ftmp2, tmp2);

	/* ftmp = z1^2*x2 - z2^2*x1 */
	felem_diff128(tmp, tmp2);
	/* tmp[i] < 2^116 + 2^64 + 8 < 2^117 */
	felem_reduce(ftmp, tmp);

	/* the formulae are incorrect if the points are equal
	 * so we check for this and do doubling if this happens */
	x_equal = felem_is_zero(ftmp);
	y_equal = felem_is_zero(ftmp3);
	z1_is_zero = felem_is_zero(z1);
	z2_is_zero = felem_is_zero(z2);
	/* In affine coordinates, (X_1, Y_1) == (X_2, Y_2) */
	if (x_equal && y_equal && !z1_is_zero && !z2_is_zero)
		{
		point_double(x3, y3, z3, x1, y1, z1);
		return;
		}

	/* ftmp5 = z1*z2 */
	felem_mul(tmp, z1, z2);
	felem_reduce(ftmp5, tmp);

	/* z3 = (z1^2*x2 - z2^2*x1)*(z1*z2) */
	felem_mul(tmp, ftmp, ftmp5);
	felem_reduce(z3, tmp);

	/* ftmp = (z1^2*x2 - z2^2*x1)^2 */
	memcpy(ftmp5, ftmp, 3 * sizeof(fslice));
	felem_square(tmp, ftmp);
	felem_reduce(ftmp, tmp);

	/* ftmp5 = (z1^2*x2 - z2^2*x1)^3 */
	felem_mul(tmp, ftmp, ftmp5);
	felem_reduce(ftmp5, tmp);

	/* ftmp2 = z2^2*x1*(z1^2*x2 - z2^2*x1)^2 */
	felem_mul(tmp, ftmp2, ftmp);
	felem_reduce(ftmp2, tmp);

	/* ftmp4 = z2^3*y1*(z1^2*x2 - z2^2*x1)^3 */
	felem_mul(tmp, ftmp4, ftmp5);
	/* tmp[i] < 4 * 2^57 * 2^57 = 2^116 */

	/* tmp2 = (z1^3*y2 - z2^3*y1)^2 */
	felem_square(tmp2, ftmp3);
	/* tmp2[i] < 4 * 2^57 * 2^57 < 2^116 */

	/* tmp2 = (z1^3*y2 - z2^3*y1)^2 - (z1^2*x2 - z2^2*x1)^3 */
	felem_diff_128_64(tmp2, ftmp5);
	/* tmp2[i] < 2^116 + 2^64 + 8 < 2^117 */

	/* ftmp5 = 2*z2^2*x1*(z1^2*x2 - z2^2*x1)^2 */
	memcpy(ftmp5, ftmp2, 3 * sizeof(fslice));
	felem_scalar64(ftmp5, 2);
	/* ftmp5[i] < 2 * 2^57 = 2^58 */

	/* x3 = (z1^3*y2 - z2^3*y1)^2 - (z1^2*x2 - z2^2*x1)^3 -
	   2*z2^2*x1*(z1^2*x2 - z2^2*x1)^2 */
	felem_diff_128_64(tmp2, ftmp5);
	/* tmp2[i] < 2^117 + 2^64 + 8 < 2^118 */
	felem_reduce(x3, tmp2);

	/* ftmp2 = z2^2*x1*(z1^2*x2 - z2^2*x1)^2 - x3 */
	felem_diff64(ftmp2, x3);
	/* ftmp2[i] < 2^57 + 2^58 + 2 < 2^59 */

	/* tmp2 = (z1^3*y2 - z2^3*y1)*(z2^2*x1*(z1^2*x2 - z2^2*x1)^2 - x3) */
	felem_mul(tmp2, ftmp3, ftmp2);
	/* tmp2[i] < 4 * 2^57 * 2^59 = 2^118 */

	/* y3 = (z1^3*y2 - z2^3*y1)*(z2^2*x1*(z1^2*x2 - z2^2*x1)^2 - x3) -
	   z2^3*y1*(z1^2*x2 - z2^2*x1)^3 */
	felem_diff128(tmp2, tmp);
	/* tmp2[i] < 2^118 + 2^120 < 2^121 */
	felem_reduce(y3, tmp2);

	/* the result (x3, y3, z3) is incorrect if one of the inputs is the
	 * point at infinity, so we need to check for this separately */

	/* if point 1 is at infinity, copy point 2 to output, and vice versa */
	copy_conditional(x3, x2, 3, z1_is_zero);
	copy_conditional(x3, x1, 3, z2_is_zero);
	copy_conditional(y3, y2, 3, z1_is_zero);
	copy_conditional(y3, y1, 3, z2_is_zero);
	copy_conditional(z3, z2, 3, z1_is_zero);
	copy_conditional(z3, z1, 3, z2_is_zero);
	}

static void affine(point P)
{
    coord z1, z2, xin, yin;
    uint128_t tmp[7];

    if (felem_is_zero(P[2])) return;
    felem_inv(z2, P[2]);
    felem_square(tmp, z2); felem_reduce(z1, tmp);
    felem_mul(tmp, P[0], z1); felem_reduce(xin, tmp);
    felem_contract(P[0], xin);
    felem_mul(tmp, z1, z2); felem_reduce(z1, tmp);
    felem_mul(tmp, P[1], z1); felem_reduce(yin, tmp);
    felem_contract(P[1], yin);
    memset(P[2], 0, sizeof(coord));
    P[2][0] = 1;
}

static void affine_x(coord out, point P)
{
    coord z1, z2, xin;
    uint128_t tmp[7];

    if (felem_is_zero(P[2])) return;
    felem_inv(z2, P[2]);
    felem_square(tmp, z2); felem_reduce(z1, tmp);
    felem_mul(tmp, P[0], z1); felem_reduce(xin, tmp);
    felem_contract(out, xin);
}

/* Multiply the given point by s */
static void point_mul(point out, point in, const felem_bytearray s)
{
    int i;
    point tmp;

    point table[16];
    memset(table[0], 0, sizeof(point));
    memmove(table[1], in, sizeof(point));
    for(i=2; i<16; i+=2) {
	point_double(table[i][0], table[i][1], table[i][2],
		     table[i/2][0], table[i/2][1], table[i/2][2]);
	point_add(table[i+1][0], table[i+1][1], table[i+1][2],
		  table[i][0], table[i][1], table[i][2],
		  in[0], in[1], in[2]);
    }
    /*
    for(i=0;i<16;++i) {
	fprintf(stderr, "table[%d]:\n", i);
	affine(table[i]);
	dump_point(NULL, table[i]);
    }
    */

    memset(tmp, 0, sizeof(point));
    for(i=0;i<21;i++) {
	u8 oh = s[20-i] >> 4;
	u8 ol = s[20-i] & 0x0f;
	point_double(tmp[0], tmp[1], tmp[2], tmp[0], tmp[1], tmp[2]);
	point_double(tmp[0], tmp[1], tmp[2], tmp[0], tmp[1], tmp[2]);
	point_double(tmp[0], tmp[1], tmp[2], tmp[0], tmp[1], tmp[2]);
	point_double(tmp[0], tmp[1], tmp[2], tmp[0], tmp[1], tmp[2]);
	if (oh) {
	    point_add(tmp[0], tmp[1], tmp[2], tmp[0], tmp[1], tmp[2],
		      table[oh][0], table[oh][1], table[oh][2]);
	}
	point_double(tmp[0], tmp[1], tmp[2], tmp[0], tmp[1], tmp[2]);
	point_double(tmp[0], tmp[1], tmp[2], tmp[0], tmp[1], tmp[2]);
	point_double(tmp[0], tmp[1], tmp[2], tmp[0], tmp[1], tmp[2]);
	point_double(tmp[0], tmp[1], tmp[2], tmp[0], tmp[1], tmp[2]);
	if (ol) {
	    point_add(tmp[0], tmp[1], tmp[2], tmp[0], tmp[1], tmp[2],
		      table[ol][0], table[ol][1], table[ol][2]);
	}
    }
    memmove(out, tmp, sizeof(point));
}

#if 0
/* Select a point from an array of 16 precomputed point multiples,
 * in constant time: for bits = {b_0, b_1, b_2, b_3}, return the point
 * pre_comp[8*b_3 + 4*b_2 + 2*b_1 + b_0] */
static void select_point(const fslice bits[4], const fslice pre_comp[16][3][4],
	fslice out[12])
	{
	fslice tmp[5][12];
	select_conditional(tmp[0], pre_comp[7][0], pre_comp[15][0], 12, bits[3]);
	select_conditional(tmp[1], pre_comp[3][0], pre_comp[11][0], 12, bits[3]);
	select_conditional(tmp[2], tmp[1], tmp[0], 12, bits[2]);
	select_conditional(tmp[0], pre_comp[5][0], pre_comp[13][0], 12, bits[3]);
	select_conditional(tmp[1], pre_comp[1][0], pre_comp[9][0], 12, bits[3]);
	select_conditional(tmp[3], tmp[1], tmp[0], 12, bits[2]);
	select_conditional(tmp[4], tmp[3], tmp[2], 12, bits[1]);
	select_conditional(tmp[0], pre_comp[6][0], pre_comp[14][0], 12, bits[3]);
	select_conditional(tmp[1], pre_comp[2][0], pre_comp[10][0], 12, bits[3]);
	select_conditional(tmp[2], tmp[1], tmp[0], 12, bits[2]);
	select_conditional(tmp[0], pre_comp[4][0], pre_comp[12][0], 12, bits[3]);
	select_conditional(tmp[1], pre_comp[0][0], pre_comp[8][0], 12, bits[3]);
	select_conditional(tmp[3], tmp[1], tmp[0], 12, bits[2]);
	select_conditional(tmp[1], tmp[3], tmp[2], 12, bits[1]);
	select_conditional(out, tmp[1], tmp[4], 12, bits[0]);
	}

/* Interleaved point multiplication using precomputed point multiples:
 * The small point multiples 0*P, 1*P, ..., 15*P are in pre_comp[],
 * the scalars in scalars[]. If g_scalar is non-NULL, we also add this multiple
 * of the generator, using certain (large) precomputed multiples in g_pre_comp.
 * Output point (X, Y, Z) is stored in x_out, y_out, z_out */
static void batch_mul(fslice x_out[4], fslice y_out[4], fslice z_out[4],
	const felem_bytearray scalars[], const unsigned num_points, const u8 *g_scalar,
	const fslice pre_comp[][16][3][4], const fslice g_pre_comp[16][3][4])
	{
	unsigned i, j, num;
	unsigned gen_mul = (g_scalar != NULL);
	fslice nq[12], nqt[12], tmp[12];
	fslice bits[4];
	u8 byte;

	/* set nq to the point at infinity */
	memset(nq, 0, 12 * sizeof(fslice));

	/* Loop over all scalars msb-to-lsb, 4 bits at a time: for each nibble,
	 * double 4 times, then add the precomputed point multiples.
	 * If we are also adding multiples of the generator, then interleave
	 * these additions with the last 56 doublings. */
	for (i = (num_points ? 28 : 7); i > 0; --i)
		{
		for (j = 0; j < 8; ++j)
			{
			/* double once */
			point_double(nq, nq+4, nq+8, nq, nq+4, nq+8);
			/* add multiples of the generator */
			if ((gen_mul) && (i <= 7))
				{
				bits[3] = (g_scalar[i+20] >> (7-j)) & 1;
				bits[2] = (g_scalar[i+13] >> (7-j)) & 1;
				bits[1] = (g_scalar[i+6] >> (7-j)) & 1;
				bits[0] = (g_scalar[i-1] >> (7-j)) & 1;
				/* select the point to add, in constant time */
				select_point(bits, g_pre_comp, tmp);
				memcpy(nqt, nq, 12 * sizeof(fslice));
				point_add(nq, nq+4, nq+8, nqt, nqt+4, nqt+8,
					tmp, tmp+4, tmp+8);
				}
			/* do an addition after every 4 doublings */
			if (j % 4 == 3)
				{
				/* loop over all scalars */
				for (num = 0; num < num_points; ++num)
					{
					byte = scalars[num][i-1];
					bits[3] = (byte >> (10-j)) & 1;
					bits[2] = (byte >> (9-j)) & 1;
					bits[1] = (byte >> (8-j)) & 1;
					bits[0] = (byte >> (7-j)) & 1;
					/* select the point to add */
					select_point(bits,
						pre_comp[num], tmp);
					memcpy(nqt, nq, 12 * sizeof(fslice));
					point_add(nq, nq+4, nq+8, nqt, nqt+4,
						nqt+8, tmp, tmp+4, tmp+8);
					}
				}
			}
		}
	memcpy(x_out, nq, 4 * sizeof(fslice));
	memcpy(y_out, nq+4, 4 * sizeof(fslice));
	memcpy(z_out, nq+8, 4 * sizeof(fslice));
	}

/******************************************************************************/
/*		       FUNCTIONS TO MANAGE PRECOMPUTATION
 */

static NISTP224_PRE_COMP *nistp224_pre_comp_new()
	{
	NISTP224_PRE_COMP *ret = NULL;
	ret = (NISTP224_PRE_COMP *)OPENSSL_malloc(sizeof(NISTP224_PRE_COMP));
	if (!ret)
		{
		ECerr(EC_F_NISTP224_PRE_COMP_NEW, ERR_R_MALLOC_FAILURE);
		return ret;
		}
	memset(ret->g_pre_comp, 0, sizeof(ret->g_pre_comp));
	ret->references = 1;
	return ret;
	}

static void *nistp224_pre_comp_dup(void *src_)
	{
	NISTP224_PRE_COMP *src = src_;

	/* no need to actually copy, these objects never change! */
	CRYPTO_add(&src->references, 1, CRYPTO_LOCK_EC_PRE_COMP);

	return src_;
	}

static void nistp224_pre_comp_free(void *pre_)
	{
	int i;
	NISTP224_PRE_COMP *pre = pre_;

	if (!pre)
		return;

	i = CRYPTO_add(&pre->references, -1, CRYPTO_LOCK_EC_PRE_COMP);
	if (i > 0)
		return;

	OPENSSL_free(pre);
	}

static void nistp224_pre_comp_clear_free(void *pre_)
	{
	int i;
	NISTP224_PRE_COMP *pre = pre_;

	if (!pre)
		return;

	i = CRYPTO_add(&pre->references, -1, CRYPTO_LOCK_EC_PRE_COMP);
	if (i > 0)
		return;

	OPENSSL_cleanse(pre, sizeof *pre);
	OPENSSL_free(pre);
	}

/******************************************************************************/
/*			   OPENSSL EC_METHOD FUNCTIONS
 */

int ec_GFp_nistp224_group_init(EC_GROUP *group)
	{
	int ret;
	ret = ec_GFp_simple_group_init(group);
	group->a_is_minus3 = 1;
	return ret;
	}

int ec_GFp_nistp224_group_set_curve(EC_GROUP *group, const BIGNUM *p,
	const BIGNUM *a, const BIGNUM *b, BN_CTX *ctx)
	{
	int ret = 0;
	BN_CTX *new_ctx = NULL;
	BIGNUM *curve_p, *curve_a, *curve_b;

	if (ctx == NULL)
		if ((ctx = new_ctx = BN_CTX_new()) == NULL) return 0;
	BN_CTX_start(ctx);
	if (((curve_p = BN_CTX_get(ctx)) == NULL) ||
		((curve_a = BN_CTX_get(ctx)) == NULL) ||
		((curve_b = BN_CTX_get(ctx)) == NULL)) goto err;
	BN_bin2bn(nistp224_curve_params[0], sizeof(felem_bytearray), curve_p);
	BN_bin2bn(nistp224_curve_params[1], sizeof(felem_bytearray), curve_a);
	BN_bin2bn(nistp224_curve_params[2], sizeof(felem_bytearray), curve_b);
	if ((BN_cmp(curve_p, p)) || (BN_cmp(curve_a, a)) ||
		(BN_cmp(curve_b, b)))
		{
		ECerr(EC_F_EC_GFP_NISTP224_GROUP_SET_CURVE,
			EC_R_WRONG_CURVE_PARAMETERS);
		goto err;
		}
	group->field_mod_func = BN_nist_mod_224;
	ret = ec_GFp_simple_group_set_curve(group, p, a, b, ctx);
err:
	BN_CTX_end(ctx);
	if (new_ctx != NULL)
		BN_CTX_free(new_ctx);
	return ret;
	}

/* Takes the Jacobian coordinates (X, Y, Z) of a point and returns
 * (X', Y') = (X/Z^2, Y/Z^3) */
int ec_GFp_nistp224_point_get_affine_coordinates(const EC_GROUP *group,
	const EC_POINT *point, BIGNUM *x, BIGNUM *y, BN_CTX *ctx)
	{
	fslice z1[4], z2[4], x_in[4], y_in[4], x_out[4], y_out[4];
	uint128_t tmp[7];

	if (EC_POINT_is_at_infinity(group, point))
		{
		ECerr(EC_F_EC_GFP_NISTP224_POINT_GET_AFFINE_COORDINATES,
			EC_R_POINT_AT_INFINITY);
		return 0;
		}
	if ((!BN_to_felem(x_in, &point->X)) || (!BN_to_felem(y_in, &point->Y)) ||
		(!BN_to_felem(z1, &point->Z))) return 0;
	felem_inv(z2, z1);
	felem_square(tmp, z2); felem_reduce(z1, tmp);
	felem_mul(tmp, x_in, z1); felem_reduce(x_in, tmp);
	felem_contract(x_out, x_in);
	if (x != NULL)
		{
		if (!felem_to_BN(x, x_out)) {
		ECerr(EC_F_EC_GFP_NISTP224_POINT_GET_AFFINE_COORDINATES,
			ERR_R_BN_LIB);
		return 0;
		}
		}
	felem_mul(tmp, z1, z2); felem_reduce(z1, tmp);
	felem_mul(tmp, y_in, z1); felem_reduce(y_in, tmp);
	felem_contract(y_out, y_in);
	if (y != NULL)
		{
		if (!felem_to_BN(y, y_out)) {
		ECerr(EC_F_EC_GFP_NISTP224_POINT_GET_AFFINE_COORDINATES,
			ERR_R_BN_LIB);
		return 0;
		}
		}
	return 1;
	}

/* Computes scalar*generator + \sum scalars[i]*points[i], ignoring NULL values
 * Result is stored in r (r can equal one of the inputs). */
int ec_GFp_nistp224_points_mul(const EC_GROUP *group, EC_POINT *r,
	const BIGNUM *scalar, size_t num, const EC_POINT *points[],
	const BIGNUM *scalars[], BN_CTX *ctx)
	{
	int ret = 0;
	int i, j;
	BN_CTX *new_ctx = NULL;
	BIGNUM *x, *y, *z, *tmp_scalar;
	felem_bytearray g_secret;
	felem_bytearray *secrets = NULL;
	fslice (*pre_comp)[16][3][4] = NULL;
	felem_bytearray tmp;
	unsigned num_bytes;
	int have_pre_comp = 0;
	size_t num_points = num;
	fslice x_in[4], y_in[4], z_in[4], x_out[4], y_out[4], z_out[4];
	NISTP224_PRE_COMP *pre = NULL;
	fslice (*g_pre_comp)[3][4] = NULL;
	EC_POINT *generator = NULL;
	const EC_POINT *p = NULL;
	const BIGNUM *p_scalar = NULL;

	if (ctx == NULL)
		if ((ctx = new_ctx = BN_CTX_new()) == NULL) return 0;
	BN_CTX_start(ctx);
	if (((x = BN_CTX_get(ctx)) == NULL) ||
		((y = BN_CTX_get(ctx)) == NULL) ||
		((z = BN_CTX_get(ctx)) == NULL) ||
		((tmp_scalar = BN_CTX_get(ctx)) == NULL))
		goto err;

	if (scalar != NULL)
		{
		pre = EC_EX_DATA_get_data(group->extra_data,
			nistp224_pre_comp_dup, nistp224_pre_comp_free,
			nistp224_pre_comp_clear_free);
		if (pre)
			/* we have precomputation, try to use it */
			g_pre_comp = pre->g_pre_comp;
		else
			/* try to use the standard precomputation */
			g_pre_comp = (fslice (*)[3][4]) gmul;
		generator = EC_POINT_new(group);
		if (generator == NULL)
			goto err;
		/* get the generator from precomputation */
		if (!felem_to_BN(x, g_pre_comp[1][0]) ||
			!felem_to_BN(y, g_pre_comp[1][1]) ||
			!felem_to_BN(z, g_pre_comp[1][2]))
			{
			ECerr(EC_F_EC_GFP_NISTP224_POINTS_MUL, ERR_R_BN_LIB);
			goto err;
			}
		if (!EC_POINT_set_Jprojective_coordinates_GFp(group,
				generator, x, y, z, ctx))
			goto err;
		if (0 == EC_POINT_cmp(group, generator, group->generator, ctx))
			/* precomputation matches generator */
			have_pre_comp = 1;
		else
			/* we don't have valid precomputation:
			 * treat the generator as a random point */
			num_points = num_points + 1;
		}
	secrets = OPENSSL_malloc(num_points * sizeof(felem_bytearray));
	pre_comp = OPENSSL_malloc(num_points * 16 * 3 * 4 * sizeof(fslice));

	if ((num_points) && ((secrets == NULL) || (pre_comp == NULL)))
		{
		ECerr(EC_F_EC_GFP_NISTP224_POINTS_MUL, ERR_R_MALLOC_FAILURE);
		goto err;
		}

	/* we treat NULL scalars as 0, and NULL points as points at infinity,
	 * i.e., they contribute nothing to the linear combination */
	memset(secrets, 0, num_points * sizeof(felem_bytearray));
	memset(pre_comp, 0, num_points * 16 * 3 * 4 * sizeof(fslice));
	for (i = 0; i < num_points; ++i)
		{
		if (i == num)
			/* the generator */
			{
			p = EC_GROUP_get0_generator(group);
			p_scalar = scalar;
			}
		else
			/* the i^th point */
			{
			p = points[i];
			p_scalar = scalars[i];
			}
		if ((p_scalar != NULL) && (p != NULL))
			{
			num_bytes = BN_num_bytes(p_scalar);
			/* reduce scalar to 0 <= scalar < 2^224 */
			if ((num_bytes > sizeof(felem_bytearray)) || (BN_is_negative(p_scalar)))
				{
				/* this is an unusual input, and we don't guarantee
				 * constant-timeness */
				if (!BN_nnmod(tmp_scalar, p_scalar, &group->order, ctx))
					{
					ECerr(EC_F_EC_GFP_NISTP224_POINTS_MUL, ERR_R_BN_LIB);
					goto err;
					}
				num_bytes = BN_bn2bin(tmp_scalar, tmp);
				}
			else
				BN_bn2bin(p_scalar, tmp);
			flip_endian(secrets[i], tmp, num_bytes);
			/* precompute multiples */
			if ((!BN_to_felem(x_out, &p->X)) ||
				(!BN_to_felem(y_out, &p->Y)) ||
				(!BN_to_felem(z_out, &p->Z))) goto err;
			memcpy(pre_comp[i][1][0], x_out, 4 * sizeof(fslice));
			memcpy(pre_comp[i][1][1], y_out, 4 * sizeof(fslice));
			memcpy(pre_comp[i][1][2], z_out, 4 * sizeof(fslice));
			for (j = 1; j < 8; ++j)
				{
				point_double(pre_comp[i][2*j][0],
					pre_comp[i][2*j][1],
					pre_comp[i][2*j][2],
					pre_comp[i][j][0],
					pre_comp[i][j][1],
					pre_comp[i][j][2]);
				point_add(pre_comp[i][2*j+1][0],
					pre_comp[i][2*j+1][1],
					pre_comp[i][2*j+1][2],
					pre_comp[i][1][0],
					pre_comp[i][1][1],
					pre_comp[i][1][2],
					pre_comp[i][2*j][0],
					pre_comp[i][2*j][1],
					pre_comp[i][2*j][2]);
				}
			}
		}

	/* the scalar for the generator */
	if ((scalar != NULL) && (have_pre_comp))
		{
		memset(g_secret, 0, sizeof g_secret);
		num_bytes = BN_num_bytes(scalar);
		/* reduce scalar to 0 <= scalar < 2^224 */
		if ((num_bytes > sizeof(felem_bytearray)) || (BN_is_negative(scalar)))
			{
			/* this is an unusual input, and we don't guarantee
			 * constant-timeness */
			if (!BN_nnmod(tmp_scalar, scalar, &group->order, ctx))
				{
				ECerr(EC_F_EC_GFP_NISTP224_POINTS_MUL, ERR_R_BN_LIB);
				goto err;
				}
			num_bytes = BN_bn2bin(tmp_scalar, tmp);
			}
		else
			BN_bn2bin(scalar, tmp);
		flip_endian(g_secret, tmp, num_bytes);
		/* do the multiplication with generator precomputation*/
		batch_mul(x_out, y_out, z_out,
			(const felem_bytearray (*)) secrets, num_points,
			g_secret, (const fslice (*)[16][3][4]) pre_comp,
			(const fslice (*)[3][4]) g_pre_comp);
		}
	else
		/* do the multiplication without generator precomputation */
		batch_mul(x_out, y_out, z_out,
			(const felem_bytearray (*)) secrets, num_points,
			NULL, (const fslice (*)[16][3][4]) pre_comp, NULL);
	/* reduce the output to its unique minimal representation */
	felem_contract(x_in, x_out);
	felem_contract(y_in, y_out);
	felem_contract(z_in, z_out);
	if ((!felem_to_BN(x, x_in)) || (!felem_to_BN(y, y_in)) ||
		(!felem_to_BN(z, z_in)))
		{
		ECerr(EC_F_EC_GFP_NISTP224_POINTS_MUL, ERR_R_BN_LIB);
		goto err;
		}
	ret = EC_POINT_set_Jprojective_coordinates_GFp(group, r, x, y, z, ctx);

err:
	BN_CTX_end(ctx);
	if (generator != NULL)
		EC_POINT_free(generator);
	if (new_ctx != NULL)
		BN_CTX_free(new_ctx);
	if (secrets != NULL)
		OPENSSL_free(secrets);
	if (pre_comp != NULL)
		OPENSSL_free(pre_comp);
	return ret;
	}

int ec_GFp_nistp224_precompute_mult(EC_GROUP *group, BN_CTX *ctx)
	{
	int ret = 0;
	NISTP224_PRE_COMP *pre = NULL;
	int i, j;
	BN_CTX *new_ctx = NULL;
	BIGNUM *x, *y;
	EC_POINT *generator = NULL;

	/* throw away old precomputation */
	EC_EX_DATA_free_data(&group->extra_data, nistp224_pre_comp_dup,
		nistp224_pre_comp_free, nistp224_pre_comp_clear_free);
	if (ctx == NULL)
		if ((ctx = new_ctx = BN_CTX_new()) == NULL) return 0;
	BN_CTX_start(ctx);
	if (((x = BN_CTX_get(ctx)) == NULL) ||
		((y = BN_CTX_get(ctx)) == NULL))
		goto err;
	/* get the generator */
	if (group->generator == NULL) goto err;
	generator = EC_POINT_new(group);
	if (generator == NULL)
		goto err;
	BN_bin2bn(nistp224_curve_params[3], sizeof (felem_bytearray), x);
	BN_bin2bn(nistp224_curve_params[4], sizeof (felem_bytearray), y);
	if (!EC_POINT_set_affine_coordinates_GFp(group, generator, x, y, ctx))
		goto err;
	if ((pre = nistp224_pre_comp_new()) == NULL)
		goto err;
	/* if the generator is the standard one, use built-in precomputation */
	if (0 == EC_POINT_cmp(group, generator, group->generator, ctx))
		{
		memcpy(pre->g_pre_comp, gmul, sizeof(pre->g_pre_comp));
		ret = 1;
		goto err;
		}
	if ((!BN_to_felem(pre->g_pre_comp[1][0], &group->generator->X)) ||
		(!BN_to_felem(pre->g_pre_comp[1][1], &group->generator->Y)) ||
		(!BN_to_felem(pre->g_pre_comp[1][2], &group->generator->Z)))
		goto err;
	/* compute 2^56*G, 2^112*G, 2^168*G */
	for (i = 1; i < 5; ++i)
		{
		point_double(pre->g_pre_comp[2*i][0], pre->g_pre_comp[2*i][1],
			pre->g_pre_comp[2*i][2], pre->g_pre_comp[i][0],
			pre->g_pre_comp[i][1], pre->g_pre_comp[i][2]);
		for (j = 0; j < 55; ++j)
			{
			point_double(pre->g_pre_comp[2*i][0],
				pre->g_pre_comp[2*i][1],
				pre->g_pre_comp[2*i][2],
				pre->g_pre_comp[2*i][0],
				pre->g_pre_comp[2*i][1],
				pre->g_pre_comp[2*i][2]);
			}
		}
	/* g_pre_comp[0] is the point at infinity */
	memset(pre->g_pre_comp[0], 0, sizeof(pre->g_pre_comp[0]));
	/* the remaining multiples */
	/* 2^56*G + 2^112*G */
	point_add(pre->g_pre_comp[6][0], pre->g_pre_comp[6][1],
		pre->g_pre_comp[6][2], pre->g_pre_comp[4][0],
		pre->g_pre_comp[4][1], pre->g_pre_comp[4][2],
		pre->g_pre_comp[2][0], pre->g_pre_comp[2][1],
		pre->g_pre_comp[2][2]);
	/* 2^56*G + 2^168*G */
	point_add(pre->g_pre_comp[10][0], pre->g_pre_comp[10][1],
		pre->g_pre_comp[10][2], pre->g_pre_comp[8][0],
		pre->g_pre_comp[8][1], pre->g_pre_comp[8][2],
		pre->g_pre_comp[2][0], pre->g_pre_comp[2][1],
		pre->g_pre_comp[2][2]);
	/* 2^112*G + 2^168*G */
	point_add(pre->g_pre_comp[12][0], pre->g_pre_comp[12][1],
		pre->g_pre_comp[12][2], pre->g_pre_comp[8][0],
		pre->g_pre_comp[8][1], pre->g_pre_comp[8][2],
		pre->g_pre_comp[4][0], pre->g_pre_comp[4][1],
		pre->g_pre_comp[4][2]);
	/* 2^56*G + 2^112*G + 2^168*G */
	point_add(pre->g_pre_comp[14][0], pre->g_pre_comp[14][1],
		pre->g_pre_comp[14][2], pre->g_pre_comp[12][0],
		pre->g_pre_comp[12][1], pre->g_pre_comp[12][2],
		pre->g_pre_comp[2][0], pre->g_pre_comp[2][1],
		pre->g_pre_comp[2][2]);
	for (i = 1; i < 8; ++i)
		{
		/* odd multiples: add G */
		point_add(pre->g_pre_comp[2*i+1][0], pre->g_pre_comp[2*i+1][1],
			pre->g_pre_comp[2*i+1][2], pre->g_pre_comp[2*i][0],
			pre->g_pre_comp[2*i][1], pre->g_pre_comp[2*i][2],
			pre->g_pre_comp[1][0], pre->g_pre_comp[1][1],
			pre->g_pre_comp[1][2]);
		}

	if (!EC_EX_DATA_set_data(&group->extra_data, pre, nistp224_pre_comp_dup,
			nistp224_pre_comp_free, nistp224_pre_comp_clear_free))
		goto err;
	ret = 1;
	pre = NULL;
 err:
	BN_CTX_end(ctx);
	if (generator != NULL)
		EC_POINT_free(generator);
	if (new_ctx != NULL)
		BN_CTX_free(new_ctx);
	if (pre)
		nistp224_pre_comp_free(pre);
	return ret;
	}

int ec_GFp_nistp224_have_precompute_mult(const EC_GROUP *group)
	{
	if (EC_EX_DATA_get_data(group->extra_data, nistp224_pre_comp_dup,
			nistp224_pre_comp_free, nistp224_pre_comp_clear_free)
		!= NULL)
		return 1;
	else
		return 0;
	}
#endif

#ifdef TESTING

#include <sys/time.h>

static u8 ctoh(char c)
{
    if (c >= '0' && c <= '9') return c-'0';
    if (c >= 'a' && c <= 'f') return c-'a'+10;
    if (c >= 'A' && c <= 'F') return c-'A'+10;
    return 0;
}

static void arg_to_bytearray(felem_bytearray ba, const char *arg)
{
    /* Convert the arg, which is a string like "1a2637c8" to a byte
     * array like 0xc8 0x37 0x26 0x1a. */
    int size = sizeof(felem_bytearray);
    int arglen = strlen(arg);
    int argsize = (arglen+1)/2;
    const char *argp = arg + arglen;
    u8 *bap = ba;

    memset(ba, 0, size);
    if (size < argsize) {
	fprintf(stderr, "Arg too long: %s\n", arg);
	exit(1);
    }

    while (argp > arg+1) {
	argp -= 2;
	*bap = (ctoh(argp[0])<<4)|(ctoh(argp[1]));
	++bap;
    }
    if (arglen & 1) {
	/* Handle the stray top nybble */
	argp -= 1;
	*bap = ctoh(argp[0]);
    }
}

static void arg_to_coord(coord c, const char *arg)
{
    felem_bytearray ba;

    arg_to_bytearray(ba, arg);
    /* Now convert it to a coord */
    bin21_to_felem(c, ba);
}

int main(int argc, char **argv)
{
    point infinity, P, Q, P2, PQ;
    felem_bytearray s;
    int i;
    struct timeval st, et;
    unsigned long el;
    int niter = 1000;

    memset(infinity, 0, sizeof(infinity));
    memset(P, 0, sizeof(P));
    memset(Q, 0, sizeof(Q));

    if (argc != 6) {
	fprintf(stderr, "Usage: %s Px Py Qx Qy s\n", argv[0]);
	exit(1);
    }

    arg_to_coord(P[0], argv[1]);
    arg_to_coord(P[1], argv[2]);
    P[2][0] = 1;
    dump_point("P", P);
    arg_to_coord(Q[0], argv[3]);
    arg_to_coord(Q[1], argv[4]);
    Q[2][0] = 1;
    dump_point("Q", Q);
    arg_to_bytearray(s, argv[5]);

    point_double(P2[0], P2[1], P2[2], P[0], P[1], P[2]);
    affine(P2);
    point_add(PQ[0], PQ[1], PQ[2], P[0], P[1], P[2], Q[0], Q[1], Q[2]);
    affine(PQ);
    dump_point("P2", P2);
    dump_point("PQ", PQ);

    gettimeofday(&st, NULL);
    for (i=0;i<niter;++i) {
	point_mul(P, P, s);
	affine(P);
    }
    gettimeofday(&et, NULL);
    el = (et.tv_sec-st.tv_sec)*1000000 + (et.tv_usec-st.tv_usec);
    fprintf(stderr, "%lu / %d = %lu us\n", el, niter, el/niter);

    dump_point("Ps", P);

    return 0;
}
#endif

/* Figure out whether there's a point with x-coordinate x on the main
 * curve.  If not, then there's one on the twist curve.  (There are
 * actually two, which are negatives of each other; that doesn't
 * matter.)  Multiply that point by seckey and set out to the
 * x-coordinate of the result. */
void ptwist_pointmul(byte out[PTWIST_BYTES], const byte x[PTWIST_BYTES],
	const byte seckey[PTWIST_BYTES])
{
    /* Compute z = x^3 + a*x + b */
    point P, Q;
    coord z, r2, Qx;
    uint128_t tmp[5];
    int ontwist;
    static const coord three = { 3, 0, 0 };
    static const coord b =
	    { 0x46d320e01dc7d6, 0x486ebc69bad316, 0x4e355e95cafedd };


    /* Convert the byte array to a coord */
    bin21_to_felem(P[0], x);

    /* Compute z = x^3 - 3*x + b */
    felem_square(tmp, P[0]); felem_reduce(z, tmp);
    felem_diff64(z, three);
    felem_mul(tmp, z, P[0]); felem_reduce(z, tmp);
    felem_sum64(z, b);

    /*
    dump_coord("z", z);
    */
    /* Compute r = P[1] = z ^ ((p+1)/4).  This will be a square root of
     * z, if one exists. */
    felem_sqrt(P[1], z);
    /*
    dump_coord("r", P[1]);
    */

    /* Is P[1] a square root of z? */
    felem_square(tmp, P[1]); felem_diff_128_64(tmp, z); felem_reduce(r2, tmp);

    if (felem_is_zero(r2)) {
	/* P(x,r) is on the curve */
	ontwist = 0;
    } else {
	/* (-x, r) is on the twist */
	ontwist = 1;
	felem_neg(P[0], P[0]);
    }
    /*
    fprintf(stderr, "ontwist = %d\n", ontwist);
    */
    memset(P[2], 0, sizeof(coord));
    P[2][0] = 1;

    /* All set.  Now do the point multiplication. */
    /*
    dump_point("P", P);
    for(i=0;i<21;++i) {
	fprintf(stderr, "%02x", seckey[20-i]);
    }
    fprintf(stderr, "\n");
    */
    point_mul(Q, P, seckey);
    affine_x(Qx, Q);
    /*
    dump_point("Q", Q);
    */

    /* Get the x-coordinate of the result, and negate it if we're on the
     * twist. */
    if (ontwist) {
	felem_neg(Qx, Qx);
    }

    /* Convert back to bytes */
    felem_to_bin21(out, Qx);
    /*
    fprintf(stderr, "out: ");
    for(i=0;i<21;++i) {
	fprintf(stderr, "%02x", out[i]);
    }
    fprintf(stderr, "\n");
    */
}

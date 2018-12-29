#ifndef ARBITRAIRE_INTERNAL_H
#define ARBITRAIRE_INTERNAL_H

#ifndef _ARB_DEBUG
#define _ARB_DEBUG 0
#endif


#ifndef _ARB_TIME
#define _ARB_TIME 0
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <stdarg.h>

/*
	Hypothesis:
	ARBT must be large enough to hold any value that the addition,
  	subtraction or multiplication of N UARBT can create.

	UARBT must be large enough to hold the base you plan on operating in

	Here we set UARBT as a uint8_t with a projected initial maximum base of
	2^8 or 256. 

	The ARBT is an int16_t which hence holds 2^15-1 = 32767.

	256 * 256 = 65536 so it's not possible for the UARBT to support base
	256 with an ARBT int16_t

	180 * 180 = 32400 which will fit inside of an ARBT

	We therefore set the maximum base as 180 for the current types

	partial carry multiplication is a * b + c + carry at any iteration,

	so is potentially 256 * 256 + 256 + 256, though the partial carry
	is likely much smaller than this

	256 * 256 + 256 + 256 = 66048 which exceeds even a 2^16 datum

	180 * 180 + 180 + 180 = 32760 which will fit inside of the 
	2^15-1 ARBT type.

	The next size up is 2147483647 (2^31-1). This huge value would
	allow us to go up to base 256 but would be a fairly large data
	type for our needs.

	Ergo, we set the maximum base at 180

	Hypothesis 2:
	
	Hardware types can be assumed to be of 8, 16, 32 and 64 bits
	for our purposes and for any reasonable base maxima of arbitraire
	in the future.

	an 8 needs a 32. therefore at 16 would need a 64 in order to go
	to the full range of the datum for the base

	2^16 = 65536 and 65536 * 65536 + 65536 + 65536 = 4295098368 and 
	2^31-1 is 2147483647, we therefore need a 2^63-1 at 922337203685477580

	Notice that even if you remove "c" and "d" from a * b + c + d
	That the number produced is still large enough to exhaust the data
	type of the next size up. 

	Theory:

		system 1:
			2^8 = 256
			2^31-1 = 2147483647
			256 * 256 + 256 + 256 = 66048
			66048 < 2147483647

		system 2:
			sqrt(2^15-1) = 181.0165
			180 * 180 + 180 + 180 = 32760
			2^15 -1 = 32767
			32760 < 32767
	Similar principles can be constructed from the above two rules
	That work for higher bases
*/
#define ARBT	int16_t
#define UARBT	uint8_t

/* basic defines */
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)>(b)?(b):(a))

/* structures */
typedef struct {		// fxdpnt fixed point type
	UARBT *number;		// The actual number
	char sign;		// Sign
	size_t lp;		// Length left of radix
	size_t len;		// Length of number (count of digits / limbs)
	size_t allocated;	// Length of allocated memory
} fxdpnt;

/* globals */
extern fxdpnt *zero;
extern fxdpnt *p5;
extern fxdpnt *one;
extern fxdpnt *two;
extern fxdpnt *ten;

extern long _arb_time;

#define _arb_time_start \
if (_ARB_TIME) \
_arb_time = clock()

#define _arb_time_end \
if (_ARB_TIME)  {\
_arb_time = clock() - _arb_time; \
fprintf(stderr, "time = %ld\n", _arb_time); }


/* function prototypes */
/* arithmetic */
fxdpnt *arb_mul(fxdpnt *, fxdpnt *, fxdpnt *, int, size_t);
fxdpnt *arb_mul2(fxdpnt *, fxdpnt *, fxdpnt *, int, size_t);
size_t arb_mul_core(UARBT *, size_t, UARBT *, size_t, UARBT *, int);
fxdpnt *arb_karatsuba_mul(fxdpnt *, fxdpnt *, fxdpnt *, int, size_t);
fxdpnt *arb_add_inter(fxdpnt *, fxdpnt *, fxdpnt *, int);
fxdpnt *arb_sub_inter(fxdpnt *, fxdpnt *, fxdpnt *, int);
fxdpnt *arb_sub(fxdpnt *, fxdpnt *, fxdpnt *, int);
fxdpnt *arb_add(fxdpnt *, fxdpnt *, fxdpnt *, int);
fxdpnt *arb_sub2(fxdpnt *, fxdpnt *, fxdpnt *, int);
fxdpnt *arb_add2(fxdpnt *, fxdpnt *, fxdpnt *, int);
fxdpnt *arb_newtonian_div(fxdpnt *, fxdpnt *, fxdpnt *, int, int);
fxdpnt *arb_div(fxdpnt *, fxdpnt *, fxdpnt *, int, size_t);
int _long_sub(UARBT *, size_t, UARBT *, size_t, int);
int _long_add(UARBT *, size_t, UARBT *, size_t, int);
/* modulus */
fxdpnt *arb_mod(fxdpnt *, fxdpnt *, fxdpnt *, int, size_t);
/* logical shift */
fxdpnt *arb_leftshift(fxdpnt *, size_t);
/* general */
void arb_flipsign(fxdpnt *);
void arb_setsign(fxdpnt *, fxdpnt *, fxdpnt *);
/* io */
void arb_print(fxdpnt *);
void arb_debug(fxdpnt *, char *);
void arb_printtrue(fxdpnt *);
void _print_core(FILE *, UARBT *, size_t, size_t, size_t);
fxdpnt *arb_str2fxdpnt(const char *);
fxdpnt *arb_parse_str(fxdpnt *, const char *);
int arb_highbase(int);
/* comparison */
int arb_compare(fxdpnt *, fxdpnt *, int);
/* copying */
void _arb_copy_core(UARBT *, UARBT *, size_t);
void *arb_copy(fxdpnt *, fxdpnt *);
/* sqrt */
fxdpnt *nsqrt(fxdpnt *, int, size_t);
fxdpnt *long_sqrt(fxdpnt *, int, size_t);
/* general */
void arb_init(fxdpnt *);
void arb_error(char *);
/* allocation */
fxdpnt *arb_expand(fxdpnt *, size_t);
void *arb_malloc(size_t);
void *arb_realloc(void *, size_t);
void *arb_calloc(size_t, size_t);
void arb_free(fxdpnt *);
/* to hardware and back */
fxdpnt *hrdware2arb(size_t);
size_t fxd2sizet(fxdpnt *, int);
/* general */
fxdpnt *remove_leading_zeros(fxdpnt *);
size_t rr(fxdpnt*);
size_t rl(fxdpnt*);
int iszero(fxdpnt*);
/* exp */
fxdpnt *arb_exp(fxdpnt *, fxdpnt *, fxdpnt *, int, size_t);
/* novelties */
fxdpnt *old_div(fxdpnt *, fxdpnt *, fxdpnt *, int, size_t);
/* opaque API handlers */
size_t arb_size(fxdpnt *);
size_t arb_allocated(fxdpnt *);
char arb_sign(fxdpnt *);
size_t arb_left(fxdpnt *);
/* generate "bignums" */
char *make_bignum(size_t, int, int);
/* function wrappers for simpler math handling */
void incr(fxdpnt **, int, char *);
void decr(fxdpnt **, int, char *);
void sub(fxdpnt *, fxdpnt *, fxdpnt **, int, char *);
void add(fxdpnt *, fxdpnt *, fxdpnt **, int, char *);
void sub2(fxdpnt *, fxdpnt *, fxdpnt **, int, char *);
void add2(fxdpnt *, fxdpnt *, fxdpnt **, int, char *);
void mul(fxdpnt *, fxdpnt *, fxdpnt **, int, size_t, char *);
void mul2(fxdpnt *, fxdpnt *, fxdpnt **, int, size_t, char *);
void divv(fxdpnt *, fxdpnt *, fxdpnt **, int, size_t, char *);
/* oddity */
int oddity(size_t);
/* */
void *_arb_memset(void *, int, size_t);

#define _internal_debug if (_ARB_DEBUG && m) { \
fprintf(stderr, __func__); \
fprintf(stderr, "()  %s", m); \
}
#define _internal_debug_end if (_ARB_DEBUG && m) \
arb_printtrue(*c);

#endif


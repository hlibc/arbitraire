#ifndef ARBITRAIRE_INTERNAL_H
#define ARBITRAIRE_INTERNAL_H

#ifndef _ARB_DEBUG
#define _ARB_DEBUG 0
#endif

#ifndef _ARB_TIME
#define _ARB_TIME 0
#endif

#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define ARBT int16_t
#define UARBT uint8_t

/* basic defines */
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

/* structures */
typedef struct {	/* fxdpnt fixed point type */
	UARBT *number;	/* The actual number */
	char sign;	/* Sign */
	size_t lp;	/* Length left of radix */
	size_t len;	/* Length of number (count of digits / limbs) */
	size_t allocated;/* Length of allocated memory */
} fxdpnt;

/* globals */
extern fxdpnt *zero;
extern fxdpnt *p5;
extern fxdpnt *one;
extern fxdpnt *two;
extern fxdpnt *three;
extern fxdpnt *ten;
extern long _arb_time;

/* function prototypes */
/* arithmetic */
fxdpnt *arb_mul(const fxdpnt *, const fxdpnt *, fxdpnt *, int, size_t);

fxdpnt *arb_mul2(const fxdpnt *, const fxdpnt *, fxdpnt *, int, size_t);
fxdpnt *arb_comba(const fxdpnt *, const fxdpnt *, fxdpnt *, int, size_t);
size_t arb_mul_core(const UARBT *, size_t, const UARBT *, size_t, UARBT *, int);
fxdpnt *arb_karatsuba_mul(const fxdpnt *, const fxdpnt *, fxdpnt *, int,
						  size_t);
fxdpnt *arb_add_inter(const fxdpnt *, const fxdpnt *, fxdpnt *, int);
fxdpnt *arb_sub_inter(const fxdpnt *, const fxdpnt *, fxdpnt *, int);
fxdpnt *arb_sub(const fxdpnt *, const fxdpnt *, fxdpnt *, int);
fxdpnt *arb_add(const fxdpnt *, const fxdpnt *, fxdpnt *, int);
fxdpnt *arb_sub2(const fxdpnt *, const fxdpnt *, fxdpnt *, int);
fxdpnt *arb_add2(const fxdpnt *, const fxdpnt *, fxdpnt *, int);
fxdpnt *arb_newtonian_div(fxdpnt *, fxdpnt *, fxdpnt *, int, int, fxdpnt *);
fxdpnt *arb_div(const fxdpnt *, const fxdpnt *, fxdpnt *, int, size_t);
/* modulus */
fxdpnt *arb_mod(const fxdpnt *, const fxdpnt *, fxdpnt *, int, size_t);
/* logical shift */
fxdpnt *arb_leftshift(fxdpnt *, size_t);
fxdpnt *arb_rightshift(fxdpnt *, size_t);
/* general */
void arb_flipsign(fxdpnt *);
void arb_setsign(const fxdpnt *, const fxdpnt *, fxdpnt *);
/* io */
void arb_print(const fxdpnt *);
void arb_debug(const fxdpnt *, char *);
void arb_printtrue(const fxdpnt *);
fxdpnt *arb_str2fxdpnt(const char *);
fxdpnt *arb_parse_str(fxdpnt *, const char *);
int arb_highbase(int);
/* comparison */
int arb_compare(const fxdpnt *, const fxdpnt *);
/* copying */
void _arb_copy_core(UARBT *, UARBT *, size_t);
fxdpnt *arb_copy(fxdpnt *, const fxdpnt *);
/* sqrt */
fxdpnt *nsqrt(fxdpnt *, int, size_t);
fxdpnt *long_sqrt(fxdpnt *, int, size_t);
/* general */
void arb_init(fxdpnt *);
void arb_error(char *);
/* allocation */
fxdpnt *arb_expand(fxdpnt *, size_t);
fxdpnt *arb_expand_inter(fxdpnt *, size_t, size_t, int);
void *arb_malloc(size_t);
void *arb_realloc(void *, size_t);
void *arb_calloc(size_t, size_t);
void arb_free(fxdpnt *);
/* to hardware and back */
fxdpnt *hrdware2arb(size_t);
size_t fxd2sizet(fxdpnt *, int);
/* general */
fxdpnt *remove_leading_zeros(fxdpnt *);
size_t rr(const fxdpnt *);
size_t rl(const fxdpnt *);
size_t le(const fxdpnt *);
int iszero(const fxdpnt *);
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
void sub(const fxdpnt *, const fxdpnt *, fxdpnt **, int, char *);
void add(const fxdpnt *, const fxdpnt *, fxdpnt **, int, char *);
void sub2(const fxdpnt *, const fxdpnt *, fxdpnt **, int, char *);
void add2(const fxdpnt *, const fxdpnt *, fxdpnt **, int, char *);
void mul(const fxdpnt *, const fxdpnt *, fxdpnt **, int, size_t, char *);
void mul2(const fxdpnt *, const fxdpnt *, fxdpnt **, int, size_t, char *);
void debugmul(const fxdpnt *, const fxdpnt *, fxdpnt **, int, size_t, char *);
void divv(const fxdpnt *, const fxdpnt *, fxdpnt **, int, size_t, char *);
/* oddity */
int oddity(size_t);
/* memset */
void *_arb_memset(void *, int, size_t);
/* zeros */
size_t count_leading_fractional_zeros(fxdpnt *);
size_t count_leading_zeros(const fxdpnt *);
/* some macros to make debugging and timing less intrusive */
#define _arb_time_start \
	if (_ARB_TIME) _arb_time = clock()

#define _arb_time_end                               \
	if (_ARB_TIME) {                                \
		_arb_time = clock() - _arb_time;            \
		fprintf(stderr, "time = %ld\n", _arb_time); \
	}

#define _internal_debug               \
	if (_ARB_DEBUG && m) {            \
		fprintf(stderr, __func__);    \
		fprintf(stderr, "()  %s", m); \
	}
#define _internal_debug_end \
	if (_ARB_DEBUG && m) arb_printtrue(*c);

#endif

#ifndef ARBITRAIRE_INTERNAL_H
#define ARBITRAIRE_INTERNAL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <stdarg.h>

/* defines */
#define ARBT unsigned char	// designed to be any type
#define MAX(a,b)      ((a)>(b)?(a):(b))
#define MIN(a,b)      ((a)>(b)?(b):(a))

/* structures */
typedef struct {		// fxdpnt fixed point type
	ARBT *number;		// The actual number
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

/* function prototypes */
/* arithmetic */
fxdpnt *arb_mul(fxdpnt *, fxdpnt *, fxdpnt *, int, size_t);
size_t arb_mul_core(ARBT *, size_t, ARBT *, size_t, ARBT *, int);
fxdpnt *arb_karatsuba_mul(fxdpnt *, fxdpnt *, fxdpnt *, int, size_t);
fxdpnt *arb_add_inter(fxdpnt *, fxdpnt *, fxdpnt *, int);
fxdpnt *arb_sub_inter(fxdpnt *, fxdpnt *, fxdpnt *, int);
fxdpnt *arb_sub(fxdpnt *, fxdpnt *, fxdpnt *, int);
fxdpnt *arb_add(fxdpnt *, fxdpnt *, fxdpnt *, int);
ARBT arb_place(fxdpnt *, fxdpnt *, size_t *, size_t);
fxdpnt *arb_newtonian_div(fxdpnt *, fxdpnt *, fxdpnt *, int, int);
fxdpnt *arb_div(fxdpnt *, fxdpnt *, fxdpnt *, int, size_t);
int _long_sub(ARBT *, size_t, ARBT *, size_t, int);
int _long_add(ARBT *, size_t, ARBT *, size_t, int);
/* modulus */
fxdpnt *arb_mod(fxdpnt *, fxdpnt *, fxdpnt *, int, size_t);
/* logical shift */
fxdpnt *arb_rightshift(fxdpnt *, size_t, int);
fxdpnt *arb_leftshift(fxdpnt *, size_t, int);
void rightshift_core(ARBT *, size_t, size_t);
/* general */
void arb_reverse(fxdpnt *);
void arb_flipsign(fxdpnt *);
void arb_setsign(fxdpnt *, fxdpnt *, fxdpnt *);
/* io */
void arb_print(fxdpnt *);
void arb_debug(fxdpnt *, char *);
void _print_core(FILE *, ARBT *, size_t, size_t, size_t, int);
fxdpnt *arb_str2fxdpnt(const char *);
fxdpnt *arb_parse_str(fxdpnt *, const char *);
int arb_highbase(int);
/* comparison */
int arb_compare(fxdpnt *, fxdpnt *, int);
/* copying */
void _arb_copy_core(ARBT *, ARBT *, size_t);
void arb_copy(fxdpnt *, fxdpnt *);
void arb_copyreverse(fxdpnt *, fxdpnt *);
/* sqrt */
fxdpnt *nsqrt(fxdpnt *, int, size_t);
fxdpnt *long_sqrt(fxdpnt *, int, size_t);
/* general */
void arb_init(fxdpnt *);
void arb_error(char *);
/* allocation */
fxdpnt *arb_alloc(size_t);
fxdpnt *arb_expand(fxdpnt *, size_t);
void *arb_malloc(size_t);
void *arb_realloc(void *, size_t);
void *arb_calloc(size_t, size_t);
void arb_free(fxdpnt *);
/* base conversion */
fxdpnt *convscaled(fxdpnt *, fxdpnt *, int, int);
/* to hardware and back */
fxdpnt *hrdware2arb(size_t);
size_t fxd2sizet(fxdpnt *, int);
/* general */
fxdpnt *remove_leading_zeros(fxdpnt *);
size_t rr(fxdpnt*);
int iszero(fxdpnt*);
/* exp */
fxdpnt *arb_exp(fxdpnt *, fxdpnt *, fxdpnt *, int, size_t);
/* novelties */
fxdpnt *old_div(fxdpnt *, fxdpnt *, fxdpnt *, int, size_t);

size_t arb_size(fxdpnt *);
size_t arb_allocated(fxdpnt *);
char arb_sign(fxdpnt *);
size_t arb_left(fxdpnt *);


char *make_bignum(size_t, int, int);

/* function wrappers for simpler math handling */
void incr(fxdpnt **, int);
void decr(fxdpnt **, int);
void sub(fxdpnt *, fxdpnt *, fxdpnt **, int);
void add(fxdpnt *, fxdpnt *, fxdpnt **, int);
void mul(fxdpnt *, fxdpnt *, fxdpnt **, int, size_t);
void divv(fxdpnt *, fxdpnt *, fxdpnt **, int, size_t);
#endif


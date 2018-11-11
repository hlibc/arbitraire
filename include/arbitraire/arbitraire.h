#ifndef INTERNAL_ARBPREC_H
#define INTERNAL_ARBPREC_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <math.h>

typedef struct fxdpnt fxdpnt;

/* globals */
extern fxdpnt *zero;
extern fxdpnt *p5;
extern fxdpnt *one;
extern fxdpnt *two;
extern fxdpnt *ten;

/* function prototypes */
/* arithmetic */
fxdpnt *arb_mul(fxdpnt *, fxdpnt *, fxdpnt *, int, size_t);
fxdpnt *arb_sub(fxdpnt *, fxdpnt *, fxdpnt *, int);
fxdpnt *arb_add(fxdpnt *, fxdpnt *, fxdpnt *, int);
fxdpnt *arb_newtonian_div(fxdpnt *, fxdpnt *, fxdpnt *, int, int);
fxdpnt *arb_div(fxdpnt *, fxdpnt *, fxdpnt *, int, size_t);
/* modulus */
fxdpnt *arb_mod(fxdpnt *, fxdpnt *, fxdpnt *, int, size_t);
/* logical shift */
fxdpnt *arb_rightshift(fxdpnt *, size_t, int);
fxdpnt *arb_leftshift(fxdpnt *, size_t, int);
/* general */
void arb_reverse(fxdpnt *);
void arb_flipsign(fxdpnt *);
void arb_setsign(fxdpnt *, fxdpnt *, fxdpnt *);
/* io */
void arb_print(fxdpnt *);
fxdpnt *arb_str2fxdpnt(const char *);
/* comparison */
int arb_compare(fxdpnt *, fxdpnt *, int);
/* copying */
void arb_copy(fxdpnt *, fxdpnt *);
void arb_copyreverse(fxdpnt *, fxdpnt *);
/* sqrt */
fxdpnt *nsqrt(fxdpnt *, int, size_t);
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
int iszero(fxdpnt*);
/* exp */
fxdpnt *arb_exp(fxdpnt *, fxdpnt *, fxdpnt *, int, size_t);
/* novelties */
fxdpnt *old_div(fxdpnt *, fxdpnt *, fxdpnt *, int, size_t);
/* opaque helper functions */
size_t arb_size(fxdpnt *);
size_t arb_allocated(fxdpnt *);
char arb_sign(fxdpnt *);
size_t arb_left(fxdpnt *);

char *make_bignum(size_t, int, int);
#endif


#ifndef ARBITRAIRE_H
#define ARBITRAIRE_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <math.h>
#include <stdarg.h>
#include <time.h>

typedef struct fxdpnt fxdpnt;

/* function prototypes */
/* arithmetic */
fxdpnt *arb_mul(const fxdpnt *, const fxdpnt *, fxdpnt *, int, size_t);
fxdpnt *arb_karatsuba_mul(const fxdpnt *, const fxdpnt *, fxdpnt *, int, size_t);
fxdpnt *arb_sub(const fxdpnt *, const fxdpnt *, fxdpnt *, int);
fxdpnt *arb_add(const fxdpnt *, const fxdpnt *, fxdpnt *, int);
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
void arb_print(fxdpnt *);
fxdpnt *arb_str2fxdpnt(const char *);
void arb_printtrue(fxdpnt *);
/* comparison */
int arb_compare(fxdpnt *, fxdpnt *);
/* copying */
fxdpnt *arb_copy(fxdpnt *, const fxdpnt *);
/* sqrt */
fxdpnt *nsqrt(fxdpnt *, int, size_t);
fxdpnt *long_sqrt(fxdpnt *, int, size_t);
fxdpnt *lhsqrt(fxdpnt *, int, size_t);
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
/* this functions generates bignums as char strings */
char *make_bignum(size_t, int, int);
/* oddity */
int oddity(size_t);
/* new */
fxdpnt *karatsuba(const fxdpnt *a, const fxdpnt *b, fxdpnt *c, int base, size_t scale);
void arb_attrs(fxdpnt *, char *);
size_t count_leading_fractional_zeros(fxdpnt *);
fxdpnt *arb_comba(const fxdpnt *, const fxdpnt *, fxdpnt *, int, size_t);
#ifdef __cplusplus
}
#endif

#endif


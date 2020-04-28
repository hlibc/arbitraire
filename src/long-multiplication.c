#include "internal.h"

/* Copyright 2017-2019 CM Graff */

/*
	arb_mul_core() is a base case "long multiplication" algorithm
	This algorithm differs from typical school book long
	multiplication in the sense that only a single "row" is
	used and the carries and summations are done as it goes
	along.

	arb_mul() is the actual interface that is intended to be used
	as an externa API as it transparently provides access to all
	of the currently implemented algorithms.

	arb_mul2() is a wrapper for arb_mul_core which provides memory
	allocation but does not strip zeros like arb_mul. arb_mul2()
	only calls the base case long multiplication algorithm.
*/

size_t arb_mul_core(const UARBT *a, size_t alen, const UARBT *b, size_t blen, UARBT *c, int base)
{
	UARBT prod = 0;
	UARBT carry = 0;
	size_t i = 0;
	size_t j = 0;
	size_t k = 0;
	size_t last = 0;
	size_t ret = 0;

	c[0] = 0;
	c[alen+blen-1] = 0;

	/* move zeros onto the solution and reduce the mag of the operands */
	for (;alen > 3 && ! a[alen-1]; ++ret) {
		c[--alen + blen -1] = 0;
	}
	for (;blen > 3 && ! b[blen-1]; ++ret) {
		c[alen + --blen -1] = 0;
	}

	/* outer loop -- first operand */
	for (i = alen; i > 0 ; i--){
		/* inner loop, second operand */
		for (j = blen, k = i + blen, carry = 0; j > 0 ; j--, k--){
			prod = a[i-1] * b[j-1] + c[k-1] + carry;
			carry = prod / base;
			c[k-1] = prod % base;
		}
		c[k-1] = carry;
	}

	return ret;
}

fxdpnt *arb_mul(const fxdpnt *a, const fxdpnt *b, fxdpnt *c, int base, size_t scale)
{
	/* use karatsuba multiplication if either operand is over 1000 digits */
	if (MAX(a->len, b->len) > 1000)
		return arb_karatsuba_mul(a, b, c, base, scale);

	c = arb_mul2(a, b, c, base, scale);
	c = remove_leading_zeros(c);
	return c;
}

fxdpnt *arb_mul2(const fxdpnt *a, const fxdpnt *b, fxdpnt *c, int base, size_t scale)
{
	fxdpnt *c2 = arb_expand(NULL, a->len + b->len);
	arb_setsign(a, b, c2);
        arb_mul_core(a->number, a->len, b->number, b->len, c2->number, base);
        c2->lp = rl(a) + rl(b);
        c2->len = MIN(rr(a) + rr(b), MAX(scale, MAX(rr(a), rr(b)))) + c2->lp;
        arb_free(c);
        return c2;
}

void mul(const fxdpnt *a, const fxdpnt *b, fxdpnt **c, int base, size_t scale, char *m)
{
	_internal_debug;
	*c = arb_mul(a, b, *c, base, scale);
	_internal_debug_end;
}

void mul2(const fxdpnt *a, const fxdpnt *b, fxdpnt **c, int base, size_t scale, char *m)
{
	_internal_debug;
	*c = arb_mul2(a, b, *c, base, scale);
	_internal_debug_end;
}

void debugmul(const fxdpnt *a, const fxdpnt *b, fxdpnt **c, int base, size_t scale, char *m)
{
	/* a tracer for following valgrind output */
	_internal_debug;
	*c = arb_mul(a, b, *c, base, scale);
	_internal_debug_end;
}



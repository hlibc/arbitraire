#include "internal.h"

/* Copyright 2017-2019 CM Graff */

/*
	arb_mul_core:

	This is a partial carry variation on the traditional "school book" long
	multiplication algorithm. Arrays are accessed via n-1 to aid in using
	unsigned types.

	A somewhat novel method for making the partial carry long 
	multiplication algorithm self-zeroing is used. In arbitraire this
	allows arb_mul_core to work with the implementation of Knuth's TAOCP 
	vol 2 algorithm D (division (src/div.c)) and allows arb_mul_core to 
	stand alone.

	An optimization is provided which tracks trailing zeros from the
	operands and moves them onto the answer. It hypothetically increases
	the speed of the multiplication when trailing zeros are present. Which
	may ultimately help karatsuba multiplication a great deal.

	The return value of arb_mul_core represents the magnitude of the
	product.

	arb_mul:

	arb_mul() is the actual interface that is intended to be used.

	arb_mul2() is a wrapper for arb_mul_core which provides memory
	allocation but does not strip zeros like arb_mul
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
		last = k;
		/* inner loop, second operand */
		for (j = blen, k = i + j, carry = 0; j > 0 ; j--, k--){
			prod = a[i-1] * b[j-1] + c[k-1] + carry;
			carry = prod / base;
			c[k-1] = prod % base;
		}
		/* self zeroing */
		if (k != last) { 
			++ret;
			c[k-1] = 0;
		}
		c[k-1] += carry;
	}

	return ret;
}

fxdpnt *arb_mul_by_one(const fxdpnt *a, const fxdpnt *b, fxdpnt *c)
{
	if (arb_compare(a, one) == 0) {
		fxdpnt *c2 = arb_expand(NULL, a->len);
		c2 = arb_copy(c2, b);
	        arb_free(c);
		c2 = remove_leading_zeros(c2);
	        return c2;

	} else if (arb_compare(b, one) == 0) {
		fxdpnt *c2 = arb_expand(NULL, b->len);
		c2 = arb_copy(c2, a);
	        arb_free(c);
		c2 = remove_leading_zeros(c2);
	        return c2;
	}
	else
		return NULL;
}


fxdpnt *arb_mul(const fxdpnt *a, const fxdpnt *b, fxdpnt *c, int base, size_t scale)
{ 
	
	//if ((c = arb_mul_by_one(a, b, c)) != NULL)
	//	return c;
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
	_internal_debug;
	*c = arb_mul(a, b, *c, base, scale);
	_internal_debug_end;
}



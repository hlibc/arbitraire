#include "internal.h"

/*
	arb_mul_core:

	A "partial carry" variation on the traditional "school book" long
	multiplication algorithm. Arrays are accessed via n-1 to allow unsigned
	types to be used therefore allowing the largest possible unsigned data
	type for indexable arrays.

	I've devised what appears to be a somewhat novel method for making the
	partial carry long multiplication algorithm zelf zeroing. Many of these
	algorithms depend on the arrays being zeroed out first, but this
	extra step alleviates the need for this. In arbitraire this is used
	for our implementation of Knuth's algorithm D (division) and allows
	arb_mul_core to stand alone.

	An optimization is provided which tracks trailing zeros from the
	operands and moves then onto the answer. This little trick was
	surprisingly difficult to get right which may be why I've not seen it
	used in other arbitrary precision methods. It hypothetically increases
	the speed of the multiplication when trailing zeros are present. Which
	may ultimately help karatsuba multiplication a great deal.

	arb_mul:
	arb_mul is the actual interface that is intended to be used.
	arb_mul2 is a wrapper for arb_mul_core which provides access to
	arb_mul_core using fxdpnts but does not strip zeros.

*/

size_t arb_mul_core(UARBT *a, size_t alen, UARBT *b, size_t blen, UARBT *c, int base)
{
	UARBT prod = 0, carry = 0;
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

fxdpnt *arb_mul(fxdpnt *a, fxdpnt *b, fxdpnt *c, int base, size_t scale)
{
	_arb_time_start;
	/* use karatsuba multiplication if either operand is over 1000 digits */
	if (MAX(a->len, b->len) > 1000)
		return arb_karatsuba_mul(a, b, c, base, scale);

	c = arb_mul2(a, b, c, base, scale);
	c = remove_leading_zeros(c);
	_arb_time_end;
	return c;
}
fxdpnt *arb_mul2(fxdpnt *a, fxdpnt *b, fxdpnt *c, int base, size_t scale)
{ 
        fxdpnt *c2 = c;
        if (a == c || b == c) {
                c2 = arb_expand(NULL, a->len + b->len);
        } else
                c2 = arb_expand(c2, a->len + b->len);
        arb_setsign(a, b, c2);
        arb_mul_core(a->number, a->len, b->number, b->len, c2->number, base);
        c2->lp = rl(a) + rl(b);
        c2->len = MIN(rr(a) + rr(b), MAX(scale, MAX(rr(a), rr(b)))) + c2->lp;
        if (a == c || b == c)
                arb_free(c);
        return c2;
}

void mul(fxdpnt *a, fxdpnt *b, fxdpnt **c, int base, size_t scale, char *m)
{
	_internal_debug;
	*c = arb_mul(a, b, *c, base, scale);
	_internal_debug_end;
}

void mul2(fxdpnt *a, fxdpnt *b, fxdpnt **c, int base, size_t scale, char *m)
{
	_internal_debug;
	*c = arb_mul2(a, b, *c, base, scale);
	_internal_debug_end;
}



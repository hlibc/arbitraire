#include "internal.h"

/*
	A "partial carry" variation on the traditional "school book" long
	multiplication algorithm. Arrays are accessed via n-1 to allow unsigned
	types to be used therefore allowing the maximum possible datum. Here,
	a size_t is used, though future developers may want to use a uint64_t
	or other fived-width datum.

	I've devised what appears to be a somewhat novel method for making the
	partial carry long multiplication algorithm zelf zeroing. Many of these
	algorithms depend on the arrays being zeroed out first, but this
	extra step alleviates the need for this. In arbitraire this is used
	for our implementation of Knuth's algorithm D (division) and allows
	arn_mul_core to stand alone.

	Multiplication does not require temporary stores of the variable and
	can be calculated without prior knowledge of the magnitude of the
	inputs. However, here we make copies of the data to allow function
	usages such as f(a, a, a) as opposed to needing f(a, b, c).
*/

size_t arb_mul_core(UARBT *a, size_t alen, UARBT *b, size_t blen, UARBT *c, int base)
{
	UARBT prod = 0, carry = 0;
	size_t i = 0;
	size_t j = 0;
	size_t k = 0;
	size_t last = 0;
	size_t ret = blen;
	c[k] = 0;
	c[alen+blen-1] = 0;
	for (i = alen; i > 0 ; i--){
		last = k;
		for (j = blen, k = i + j, carry = 0; j > 0 ; j--, k--){
			prod = (a[i-1]) * (b[j-1]) + (c[k-1]) + carry;
			carry = prod / base;
			c[k-1] = (prod % base);
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
	fxdpnt *c2 = arb_expand(NULL, a->len + b->len);
	arb_setsign(a, b, c2); 
	arb_mul_core(a->number, a->len, b->number, b->len, c2->number, base);
	c2->lp = a->lp + b->lp;
	c2->len = MIN(rr(a) + rr(b), MAX(scale, MAX(rr(a), rr(b)))) + c2->lp;
	c2 = remove_leading_zeros(c2);
	if (c)
		arb_free(c);
	return c2;
}

void mul(fxdpnt *a, fxdpnt *b, fxdpnt **c, int base, size_t scale, char *m)
{
	_internal_debug;
	*c = arb_mul(a, b, *c, base, scale);
	_internal_debug_end;
}

#include "internal.h"

/*
	This is a basic modulus operation which naturally handles fractional
	arguments using the formula: modulus(a, b) = a - (b * (a / b))
*/


fxdpnt *arb_mod(const fxdpnt *a, const fxdpnt *b, fxdpnt *c, int base, size_t scale)
{
	size_t newscale = MAX(a->len, b->len + scale);
	fxdpnt *tmp = arb_expand(NULL, newscale);
	tmp = arb_div(a, b, tmp, base, scale);
	tmp = arb_mul(tmp, b, tmp, base, newscale);
	c = arb_sub(a, tmp, c, base);
	arb_free(tmp);
	return c;
}


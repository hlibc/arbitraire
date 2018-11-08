#include <arbitraire/arbitraire.h>

/*
	This is a basic modulus operation which naturally handles fractional
	arguments using the formula: modulus(a) = a - (b * (a / b))
*/


fxdpnt *arb_mod(fxdpnt *a, fxdpnt *b, fxdpnt *c, int base, size_t scale)
{
	size_t newscale = MAX(a->len, b->len + scale);
	fxdpnt *tmp = arb_expand(NULL, newscale);
	tmp = arb_div(a, b, tmp, base, scale);
	tmp = arb_mul(tmp, b, tmp, base, newscale);
	c = arb_sub(a, tmp, c, base);
	free(tmp);
	return c;
}


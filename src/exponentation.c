#include "internal.h"
/* 
	Copyright 2018  Christopher M. Graff
*/

fxdpnt *arb_exp(fxdpnt *a, fxdpnt *b, fxdpnt *c, int base, size_t scale)
{
	fxdpnt *t;
	fxdpnt *o;
	size_t e;
	size_t z;

	t = arb_expand(NULL, a->len);
	o = arb_expand(NULL, a->len);

	e = fxd2sizet(b, 10);
	z = MIN(rr(a)*e, MAX(scale, rr(a)));

	arb_copy(o, a);
	for (;!(e & 1); e/=2)
		o = arb_mul(o, o, o, base, z);

	arb_copy(t, o);
	for (e/=2; e ; e/=2)
	{
		o = arb_mul(o, o, o, base, z);
		if ((e%2) == 1)
			t = arb_mul(t, o, t, base, z);
	}
	arb_free(c);
	arb_free(o);
	c = t;
	return t;
}



#include "internal.h"

/*
	Karatsuba multiplication

	arb_add2 and arb_sub2 are simply variants which do not strip zeros.
*/

static fxdpnt *karatsuba(fxdpnt *a, fxdpnt *b, fxdpnt *c, int base)
{
	static size_t iterations = 0;
	++iterations;
	if (b->len < 100 || a->len < 100) {
		return arb_mul2(b, a, c, base, 10);
	}

	size_t m = ((MIN(a->len, b->len)+1) / 2);
	
	/* these variables all get their memory from the calling functions */
	fxdpnt *z1, *z2, *z3, *z4, *z6, *z7, *z8;
	z1 = z2 = z3 = z4 = z6 = z7 = z8 = NULL;

	fxdpnt *z5 = arb_expand(NULL, 0);
	/* stack variables as/with pointers to valid fxdpnts */
	fxdpnt x1[1] = { 0 };
	fxdpnt y1[1] = { 0 };
	fxdpnt x0[1] = { 0 };
	fxdpnt y0[1] = { 0 };

	x1->number = a->number;
	x1->lp = x1->len = a->len - m;
	y1->number = b->number;
	y1->lp = y1->len = b->len - m;
	x0->number = a->number + a->len - m;
	x0->lp = x0->len = m;
	y0->number = b->number + b->len - m;
	y0->lp = y0->len = m;


	z1 = karatsuba(x1, y1, z1, base);
	z4 = karatsuba(x0, y0, z4, base);
	
	z2 = arb_add2(x1, x0, z2, base);
	z3 = arb_add2(y1, y0, z3, base);
	z5 = karatsuba(z2, z3, z5, base);

	z6 = arb_sub2(z5, z1, z6, base);
	z7 = arb_sub2(z6, z4, z7, base);
	z7 = arb_expand(z7, z7->len + m);
	z7->lp += m;
	z7->len += m;

	z1 = arb_expand(z1, z1->len + 2 * m);
	z1->lp += 2 * m;
	z1->len += 2 * m;
	z8 = arb_add2(z1, z7, z8, base);
	c = arb_add2(z8, z4, c, base);

	arb_free(z1);
	arb_free(z2);
	arb_free(z3);
	arb_free(z4);
	arb_free(z5);
	arb_free(z6);
	arb_free(z7);
	arb_free(z8);
	fprintf(stderr, "total iterations %zu\n", iterations);
	return c;
}

fxdpnt *arb_karatsuba_mul(fxdpnt *a, fxdpnt *b, fxdpnt *c, int base, size_t scale)
{
	fxdpnt *a2 = arb_expand(NULL, MAX(scale, a->len));
	fxdpnt *b2 = arb_expand(NULL, MAX(scale, b->len));
	fxdpnt *c2 = arb_expand(NULL, a2->len + b2->len + 3);
	arb_copy(a2, a);
	arb_copy(b2, b);
	c2 = karatsuba(a2, b2, c2, base);
	arb_setsign(a, b, c2);
	c2->lp = a2->lp + b2->lp;
	c2->len = MIN(rr(a2) + rr(b2), MAX(scale, MAX(rr(a2), rr(b2)))) + c2->lp;
	c2 = remove_leading_zeros(c2);
	arb_free(a2);
	arb_free(b2);
	if (c)
		arb_free(c);
	return c2;
}


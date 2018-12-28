#include "internal.h"
static fxdpnt *arb_karatsuba_mul_core(fxdpnt *x, fxdpnt *y, fxdpnt *z, int base)
{
	size_t size = 0;
	if (y->len == 1) {
		mul2(x, y, &z, base, size, 0);
		return z;
	} else if (x->len == 1) {
		mul2(y, x, &z, base, size, 0);
		return z;
	}

	size_t m = ((MIN(x->len, y->len)) / 2);

	fxdpnt *x1 = arb_expand(NULL, x->len - m + size);
	fxdpnt *y1 = arb_expand(NULL, y->len - m + size);
	fxdpnt *x0 = arb_expand(NULL, m	+size);
	fxdpnt *y0 = arb_expand(NULL, m + size);


	_arb_copy_core(x1->number, x->number, (x->len - m));
	_arb_copy_core(y1->number, y->number, (y->len - m));
	_arb_copy_core(x0->number, x->number + x->len - m, m);
	_arb_copy_core(y0->number, y->number + y->len - m, m);

	x1->lp = x1->len = x->len - m;
	y1->lp = y1->len = y->len - m;
	x0->lp = x0->len = m;
	y0->lp = y0->len = m;

	fxdpnt *z1 = arb_expand(NULL, size);
	fxdpnt *z2 = arb_expand(NULL, size);
	fxdpnt *z3 = arb_expand(NULL, size);
	fxdpnt *z4 = arb_expand(NULL, size);
	fxdpnt *z5 = arb_expand(NULL, size);


	z1 = arb_karatsuba_mul_core(x1, y1, z1, base);

	z2 = arb_add2(x1, x0, z2, base);
	z3 = arb_add2(y1, y0, z3, base);
	z4 = arb_karatsuba_mul_core(x0, y0, z4, base);
	z5 = arb_karatsuba_mul_core(z2, z3, z5, base);


	fxdpnt *z6 = arb_expand(NULL, size);
	fxdpnt *z7 = arb_expand(NULL, size);
	z6 = arb_sub(z5, z1, z6, base);
	z7 = arb_sub(z6, z4, z7, base);
	z7 = arb_expand(z7, z7->len + m);
	z7->lp += m;
	z7->len += m;

	z1 = arb_expand(z1, z1->len + 2 * m);
	z1->lp += 2 * m;
	z1->len += 2 * m;

	fxdpnt *z8 = arb_expand(NULL, size);
	z8 = arb_add2(z1, z7, z8, base);
	z = arb_add2(z8, z4, z, base);

	arb_free(x0);
	arb_free(y0);
	arb_free(x1);
	arb_free(y1);
	arb_free(z1);
	arb_free(z2);
	arb_free(z3);
	arb_free(z4);
	arb_free(z5);
	arb_free(z6);
	arb_free(z7);
	arb_free(z8);

	return z;
}

fxdpnt *arb_karatsuba_mul(fxdpnt *a, fxdpnt *b, fxdpnt *c, int base, size_t scale)
{
	fxdpnt *a2 = arb_expand(NULL, MAX(scale, a->len));
	fxdpnt *b2 = arb_expand(NULL, MAX(scale, b->len));
	fxdpnt *c2 = arb_expand(NULL, a2->len + b2->len + 3);
	arb_copy(a2, a);
	arb_copy(b2, b);
	c2 = arb_karatsuba_mul_core(a2, b2, c2, base);
	arb_setsign(a, b, c2);
	c2->lp = a2->lp + b2->lp;
	c2->len = MIN(rr(a2) + rr(b2), MAX(scale, MAX(rr(a2), rr(b2)))) + c2->lp;
	c2 = remove_leading_zeros(c2);
	 //c2->len--;
	arb_free(a2);
	arb_free(b2);
	if (c)
		arb_free(c);
	return c2;
}

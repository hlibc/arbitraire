#include <arbitraire/arbitraire.h>

fxdpnt *arb_newtonian_div(fxdpnt *a, fxdpnt *b, fxdpnt *c, int base, int scale)
{
	size_t s1 = 0;
        //size_t s2 = 2;
	fxdpnt *g = arb_expand(NULL, a->len);
	fxdpnt *g1 = arb_expand(NULL, 1); 
	fxdpnt *hold = arb_expand(NULL, a->len);

	g->lp = 0;
	memset(g->number, 0, a->len * sizeof(ARBT));
	g->number[a->len -1] = 5;
	g->len = a->len;

	s1 = MAX(MAX(rr(a), rr(b)), (size_t)scale);
	s1 += (s1/4);
	for (;;)
	{ 
		hold = arb_mul(b, g, hold, base, s1);
		hold = arb_sub(two, hold, hold, base);
		g1 = arb_mul(g, hold, g1, base, s1);
		if (arb_compare(g, g1, base) == 0) 
			break;
		arb_copy(g, g1);
	}
	//c = arb_expand(c, g->len + a->len);
	c = arb_mul(g, a, c, base, s1);
	c = arb_div(c, one, c, base, scale);
	return c;
}


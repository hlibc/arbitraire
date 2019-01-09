#include "internal.h"

fxdpnt *arb_newtonian_div(fxdpnt *a, fxdpnt *b, fxdpnt *c, int base, int scale)
{
	size_t apx = MAX(a->len, b->len) * 2;
	fxdpnt *g = arb_expand(NULL, apx);
	fxdpnt *g1 = arb_expand(NULL, apx); 
	fxdpnt *hold = arb_expand(NULL, apx); 
	_arb_memset(g->number, 0, apx); 
	g->number[apx -1] = 1; 
	g->lp = 0;
	for (;;)
	{ 
		hold = arb_mul(b, g, hold, base, scale);
		hold = arb_sub(two, hold, hold, base);
		g1 = arb_mul(g, hold, g1, base, scale);
	
		if (arb_compare(g, g1, base) == 0) 
		{
			break;
		}
		g = arb_copy(g, g1);
	} 
	c = arb_mul(g, a, c, base, scale); 
	/* this is just to truncate the scale */
	//c = arb_div(c, one, c, base, scale);
	return c;
}


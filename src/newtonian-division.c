#include "internal.h"

/* Division using Newton's method. This function is not intended to be used in production
 * and is here for research purposes only.
 *
 * the initial guess can be overridded with the final argument so that the function can
 * be more easily studied
 */

fxdpnt *arb_newtonian_div(fxdpnt *a, fxdpnt *b, fxdpnt *c, int base, int scale, fxdpnt *override)
{ 
	fxdpnt *g = NULL;
	fxdpnt *g1 = NULL;
	fxdpnt *hold = NULL;

	/* try to get a close logical aproximation of 1/d to populate the initial guess */
        g = arb_expand_inter(g, MAX(a->len, b->len), 0);
	g->number[MAX(a->len, b->len) -1] = 1;
	g->len = MAX(a->len, b->len);
	g->lp = 0;

	if (override) {
		g = arb_copy(g, override);
	}

	for (;;)
	{ 
		hold = arb_mul(b, g, hold, base, scale);
		hold = arb_sub(two, hold, hold, base);
		g1 = arb_mul(g, hold, g1, base, scale);
	
		if (arb_compare(g, g1) == 0) {
			break;
		}
		g = arb_copy(g, g1);
	} 
	c = arb_mul(g, a, c, base, scale); 

	return c;
}


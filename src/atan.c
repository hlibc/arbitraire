#include "internal.h"



fxdpnt *arb_atan(fxdpnt *x, int base, size_t scale)
{
	
	fxdpnt *p2 = arb_str2fxdpnt(".2");
	fxdpnt *np2 = arb_str2fxdpnt("-.2");
	fxdpnt *c1 = arb_str2fxdpnt(".7853981633974483096156608");
	fxdpnt *c2 = arb_str2fxdpnt(".7853981633974483096156608458198757210492");
	fxdpnt *c3 = arb_str2fxdpnt(".785398163397448309615660845819875721049292349843776455243736");
	fxdpnt *c4 = arb_str2fxdpnt(".1973955598498807583700497");
	fxdpnt *c5 = arb_str2fxdpnt(".1973955598498807583700497651947902934475");
	fxdpnt *c6 = arb_str2fxdpnt(".197395559849880758370049765194790293447585103787852101517688");
	fxdpnt *a = arb_str2fxdpnt("0");
	fxdpnt *f = arb_str2fxdpnt("0");
	fxdpnt *m = arb_str2fxdpnt("1");
	int comp = 0;

	/* Negative x? */

	
	comp = arb_compare(x, zero, base);
	if (comp == -1 || comp == 0)
	{
		//m = -1;
		arb_flipsign(m);
		arb_flipsign(x);
	}

	/* Special case and for fast answers */
	comp = arb_compare(x, one, base);
	if (comp == 0)
	{
		if (scale <= 25)
			return c1;
		if (scale <= 40)
			return c2;
		if (scale <= 60)
			return c3;
	}

	comp = arb_compare(x, p2, base);
	if (comp == 0)
	{
		if (scale <= 25)
			return c4;
		if (scale <= 40)
			return c5;
		if (scale <= 60)
			return c6;
	}
	/* save the scale */
	size_t z = scale; 
	/* Calculate atan of a known number. */
	comp = arb_compare(x, p2, base);
	if (comp == -1) {
		scale = z+5;
		a = arb_atan(p2, base, scale);
	}
	/* Precondition x. */
	scale = z + 3;
	fxdpnt *t1 = arb_str2fxdpnt("0");
	fxdpnt *t2 = arb_str2fxdpnt("0");
	while (1)
	{
		comp = arb_compare(x, p2, base);
		if (comp != 1)
			break;
		incr(&f, base, 0);
		sub(x, p2, &t1, base, 0);
		mul(x, p2, &t2, base, scale, 0);
		incr(&t2, base, 0);
		divv(t1, t2, &x, base, scale, 0);
		
	}
	/* Initialize the series. */
	fxdpnt *n = NULL;
	fxdpnt *v = NULL;
	fxdpnt *xx = NULL;
	fxdpnt *s = NULL;
	fxdpnt *e = NULL;
	v = arb_copy(v, x);
	n = arb_copy(n, x);
	xx = arb_copy(xx, x);
	arb_flipsign(xx);
	mul(xx, x, &s, base, scale, 0);
	
	/* Calculate the series. */
	fxdpnt *i = arb_str2fxdpnt("3");
	for (; 1 ;) {
		fprintf(stderr, "do we get here\n");
		
		arb_print(e);
		mul(n, s, &n, base, scale, 0);
		divv(n, i, &e, base, scale, 0);
		//comp = arb_compare(e, zero, base);
		//if (comp == 0) {
		if (iszero(e)== 0) {

			fprintf(stderr, "final\n");
			scale = z;
			mul(f, a, &t1, base, scale, 0);
			add(t1, v, &t1, base, 0);
			divv(t1, m, &t1, base, scale, 0);
			return t1;
		}
		add(v, e, &v, base, 0);
		add(i, two, &i, base, 0);
	}
}

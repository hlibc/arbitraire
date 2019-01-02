#include "internal.h"



fxdpnt *arb_atan(fxdpnt *x, int base, size_t scale)
{
	
	fxdpnt *p2 = arb_str2fxdpnt(".2");
	fxdpnt *c1 = arb_str2fxdpnt(".7853981633974483096156608");
	fxdpnt *c2 = arb_str2fxdpnt(".7853981633974483096156608458198757210492");
	fxdpnt *c3 = arb_str2fxdpnt(".785398163397448309615660845819875721049292349843776455243736");
	fxdpnt *c4 = arb_str2fxdpnt(".1973955598498807583700497");
	fxdpnt *c5 = arb_str2fxdpnt(".1973955598498807583700497651947902934475");
	fxdpnt *c6 = arb_str2fxdpnt(".197395559849880758370049765194790293447585103787852101517688");
	fxdpnt *a = NULL;
	fxdpnt *f = arb_str2fxdpnt("0");
	int comp = 0;

	/* Negative x? */

	int m = 1;
	comp = arb_compare(x, zero, base);
	if (comp == -1 || comp == 0)
	{
		m = -1;
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
	while (1)
	{
		comp = arb_compare(x, p2, base);
		if (comp != 1)
			break;
		incr(&f, base, 0);
		
	}
	/* Initialize the series. */
	/* Calculate the series. */
}

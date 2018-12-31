#include "internal.h"

fxdpnt *arb_sin(fxdpnt *x, int base, size_t scale)
{

	fxdpnt *i = arb_str2fxdpnt("0");
	fxdpnt *j = arb_str2fxdpnt("0");
	fxdpnt *d = arb_str2fxdpnt("1");
	fxdpnt *t = arb_str2fxdpnt("0");
	fxdpnt *y = arb_str2fxdpnt("0");
	fxdpnt *z = arb_str2fxdpnt("0");
	fxdpnt *sign = arb_str2fxdpnt("1");
	int c = 0;

	for ( ; 1 ; )
	{
		d = arb_copy(d, one);
		mul(two, i, &j, base, scale, "j = ");
		add(j, one, &j, base, "j = ");
		
		do {
			if ((c = arb_compare(j, zero, base) != 1))
				break;
			divv(x, j, &t, base, scale, "t = ");
			mul(d, t, &d, base, scale, "d = ");
			decr(&j, base, "j = ");
		}while (1);
		
		mul(d, sign, &t, base, scale, "t ");
		add(y, t, &y, base, "y = ");
		arb_flipsign(sign);
		
		if ((c = arb_compare(y, z, base) == 0))
			break;
		z = arb_copy(z, y);
		incr(&i, base, "i = ");
	}
	arb_free(i);
	arb_free(j);	
	arb_free(d);
	arb_free(t);
	arb_free(y);
	arb_free(z);
	arb_free(sign);
	return y;
}

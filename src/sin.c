#include "internal.h"

/* this is a special type of taylor series expansion which
   creates a temporary middle term 

*/

fxdpnt *arb_series(fxdpnt *x, int base, size_t scale, int needsone, int hyperbol)
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
		if (needsone)
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
		if (hyperbol)
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
	arb_free(z);
	arb_free(sign);
	arb_free(x);
	return y;
}

fxdpnt *arb_sin(fxdpnt *x, int base, size_t scale)
{
	return arb_series(x, base, scale, 1, 1);
}

fxdpnt *arb_cos(fxdpnt *x, int base, size_t scale)
{
	return arb_series(x, base, scale, 0, 1);
}

fxdpnt *arb_cosh(fxdpnt *x, int base, size_t scale)
{
	return arb_series(x, base, scale, 0, 0);
}

fxdpnt *arb_sinh(fxdpnt *x, int base, size_t scale)
{
	return arb_series(x, base, scale, 1, 1);
}


fxdpnt *arb_exp_trans(fxdpnt *x)
{
/*
        int i;
        int n = 0; 
        double y = 0;
        double d = 0;
        double z = 0;


        if (x == 1.0) {
                y = EULER;
        } else if (x < 0) {
                y = 1.0 / myexp(-x);
        } else {
                n = 2;
                y = 1.0 + x;
                while ( 1 )
                {
                        d = x; 
                        for (i = 2; i <= n; i++)
                        {
                                d *= x / i;
                        } 
                        y += d;
                        n++; 
                        if ( y == z )
                                break;
                        z = y;
                }
        }
        return y;
	*/
	return x;
}

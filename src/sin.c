#include "internal.h"

fxdpnt *arb_sin(fxdpnt *x, int base, size_t scale)
{
/*
        int i;
        int j;
        int sign = 1;
        double d;
        double z = 0;
        double y = 0;

        while ( x >= (2*PI))
                x -= (2*PI);

        while ( x < 0 )
                x += (2*PI);
*/


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
		mul(two, i, &j, base, scale, "j = ");
		add(j, one, &j, base, "j = ");
		
		for (;1;)
		{
			if ((c = arb_compare(j, zero, base) != 1))
				break;
				
			divv(x, j, &t, base, scale, "t = ");
			mul(d, t, &d, base, scale, "d = ");
			decr(&j, base, "j = ");
		}
		
		mul(d, sign, &t, base, scale, "t ");
		add(y, t, &y, base, "y = ");
		arb_flipsign(sign);
		
		if ((c = arb_compare(y, z, base) == 0))
			break;
		z = arb_copy(z, y);
		incr(&i, base, "i = ");
	}
	return y;
/*
        for (i = 0; 1; i++)
        {
                d = 1.0;
                for (j = (2*i) + 1 ; j > 0; j--)
                {
                        d *= (x / j);
                }
                y += d * sign;
                sign = -sign;
                if ( y == z )
                        break;
                z = y;
        }

        return y;
*/
}

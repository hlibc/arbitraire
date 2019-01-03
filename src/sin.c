#include "internal.h"


fxdpnt *arb_series(fxdpnt *x, int base, size_t scale, int needsone, int hyperbol)
{
	size_t oldscale = scale;
	scale *= 2;
	fxdpnt *i = arb_str2fxdpnt("0");
	fxdpnt *j = arb_str2fxdpnt("0");
	fxdpnt *d = arb_str2fxdpnt("1");
	fxdpnt *t = arb_str2fxdpnt("0");
	fxdpnt *y = arb_str2fxdpnt("0");
	fxdpnt *z = arb_str2fxdpnt("0");

	int c = 0;
	size_t sign = 0;

	do {
		d = arb_copy(d, one);
		
		mul(two, i, &j, base, scale, "j = ");
		if (needsone)
			incr(&j, base, 0);
	
		do {
			if ((c = arb_compare(j, zero, base) != 1))
				break;
			divv(x, j, &t, base, scale, "t = ");
			mul(d, t, &d, base, scale, "d = ");
			decr(&j, base, "j = ");
		}while (1);

		if (hyperbol && oddity(sign++))
			arb_flipsign(d);

		add(y, d, &y, base, "y = ");
	
		if ((c = arb_compare(y, z, base) == 0))
			break;
		z = arb_copy(z, y);
		incr(&i, base, "i = ");
		
	} while (1);

	arb_free(i);
	arb_free(j);	
	arb_free(d);
	arb_free(t);
	arb_free(z);
	arb_free(x);
	divv(y, one, &y, base, oldscale, 0);
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
fxdpnt *arb_exp_trans(fxdpnt *x, int base, size_t scale)
{
        fxdpnt *i = arb_str2fxdpnt("0");
        fxdpnt *n = arb_str2fxdpnt("2");
        fxdpnt *y = arb_str2fxdpnt("0");
        fxdpnt *d = arb_str2fxdpnt("0");
        fxdpnt *z = arb_str2fxdpnt("0");
        fxdpnt *t = arb_str2fxdpnt("0");

        add(one, x, &y, base, "y = ");
        int c = 0;
        do {
                d = arb_copy(d, x);
                i = arb_copy(i, two);
                while (1) {
                        c = arb_compare(i, n, base);
                        if (c == 1)
                                break;

                        divv(x, i, &t, base, scale, "t = ");
                        mul(d, t, &d, base, scale, "d = ");
                        incr(&i, base, "i = ");
                }
                add(y, d, &y, base, "y = ");
                incr(&n, base, "n = ");
                if ((c = arb_compare(y, z, base)) == 0)
                        break;
                z = arb_copy(z, y);
        }while (1);

	arb_free(i);
	arb_free(n);
	arb_free(d);
	arb_free(z);
	arb_free(t);
	arb_free(x);

	return y;
}

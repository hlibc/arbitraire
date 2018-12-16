#include "internal.h"

/*
	This function is still under construction 


	Karatsuba multiplication

	Karatsuba multiplication takes a multiplication of the form:
		12345678 * 55559999
	And transforms it into the form:
 	       1234*5555*10^8 + (1234*9999 + 5678*5555)*10^4 + 5678*9999

	This version of karatsuba multiplication is not optimized.

	Some ideas for optimization are:

		1> eliminate data copying using pointers.
		3> use a simpler addition algorithm which does not support
		   fractional arguments.
		4> use a simpler multiplication algorithm which does not
		   support fractional arguments.
*/

/* it's probably possible to modify the normal copy function to do something like this */
size_t karatsuba_copies(fxdpnt *a, fxdpnt *b, size_t half_or_len, size_t i)
{
	size_t j = 0;
	for(;i < half_or_len;++i, ++j)
	{
		if (i < b->len)
			a->number[j] = b->number[i];
		else
			a->number[j] = 0;
	}
	return i;
}
	

size_t split(fxdpnt *a, fxdpnt *b, fxdpnt **aa, fxdpnt **bb, fxdpnt **cc, fxdpnt **dd)
{
	/* pass in two fxdpnts and four NULL new fxdpnts */
	size_t half = 0;
	size_t compensated_mag = 0;
	size_t len = a->len;
	size_t i = 0;
	
	if (a->len > b->len) {
		len = a->len;
		compensated_mag += (a->len - b->len);
	} else if (b->len > a->len) {
		len = b->len;
		compensated_mag += (b->len - a->len);
	}
	
	if (oddity(len)) {
		len += 1;
		compensated_mag += 2;
	}

	half = len / 2;
	*aa = arb_expand(NULL, half);
	*bb = arb_expand(NULL, half);
	*cc = arb_expand(NULL, half);
	*dd = arb_expand(NULL, half);
	i = karatsuba_copies(*aa, a, half, 0);
	i = karatsuba_copies(*bb, a, len, i);
	//i = 0;
	i = karatsuba_copies(*cc, b, half, 0);
	i = karatsuba_copies(*dd, b, len, i);
	(*aa)->len = (*aa)->lp = (*bb)->len = (*bb)->lp = half;
	(*cc)->len = (*cc)->lp = (*dd)->len = (*dd)->lp = half;
	return compensated_mag;
}

fxdpnt *karatsuba2(fxdpnt *a, fxdpnt *b, fxdpnt *c, int base, size_t scale)
{
	//c = c;
	fxdpnt *aa = NULL;
	fxdpnt *bb = NULL;
	fxdpnt *cc = NULL;
	fxdpnt *dd = NULL;
	fxdpnt *total = NULL;
	fxdpnt *midtot = NULL;
	fxdpnt *mid1 = NULL;
	fxdpnt *mid2 = NULL;
	fxdpnt *end = NULL;
	fxdpnt *front = NULL;
	total = c;
	size_t comp = split(a, b, &aa, &bb, &cc, &dd);
	
	mul2(aa, cc, &front, base, scale, 0);
	arb_expand(front, ((aa->len + bb->len) * 2));
	front->len = front->lp = ((aa->len + bb->len) * 2);
	mul2(aa, dd, &mid1, base, scale, 0);
	arb_expand(mid1, (aa->len + bb->len + aa->len));
	mid1->len = mid1->lp = ((aa->len + bb->len + aa->len));
	mul2(bb, cc, &mid2, base, scale, 0);
	arb_expand(mid2, (aa->len + bb->len + aa->len));
	mid2->len = mid2->lp = ((aa->len + bb->len + aa->len));
	add2(mid1, mid2, &midtot, base, 0);
	add2(midtot, front, &total, base, 0);
	mul2(bb, dd, &end, base, scale, 0);
	add2(end, total, &total, base, 0);
	total->len = total->lp = (total->len - comp);
	arb_free(aa);
	arb_free(bb);
	arb_free(cc);
	arb_free(dd);
	arb_free(end);
	arb_free(front);
	arb_free(midtot);
	arb_free(mid1);
	arb_free(mid2);
	return total;
}


fxdpnt *karatsuba(fxdpnt *a, fxdpnt *b, fxdpnt *c, int base, size_t scale)
{
        fxdpnt *c2 = c;
        if (a == c || b == c) {
                c2 = arb_expand(NULL, a->len + b->len);
        } else
                c2 = arb_expand(c2, a->len + b->len);
        
	size_t t = rl(a);
	size_t u = rl(b);
	size_t v = MIN(rr(a) + rr(b), MAX(scale, MAX(rr(a), rr(b)))) + t + u;

        c2 = karatsuba2(a, b, c2, base, scale);
	arb_setsign(a, b, c2);
        c2->lp = t + u;
 
	c2->len = v;
        if (a == c || b == c)
                arb_free(c);
	c2 = remove_leading_zeros(c2);
        return c2;
}


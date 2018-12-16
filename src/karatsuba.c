#include "internal.h"

/*
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

size_t split(fxdpnt *a, fxdpnt *b, fxdpnt **aa, fxdpnt **bb, fxdpnt **cc, fxdpnt **dd)
{
	/* pass in two fxdpnts and four NULL new fxdpnts */
	/* theoretical future: free the first two fxdpnt */
	size_t half = 0;
	size_t compensated_mag = 0;
	size_t alen = a->len;
	size_t blen = b->len;
	size_t len = alen;
	
	if (alen > blen) {
		len = alen;
		compensated_mag += (alen - blen);
	} else if (blen > alen) {
		len = blen;
		compensated_mag += (blen - alen);
	}
	
	if (oddity(len)) {
		len += 1;
		compensated_mag += 2;
	}

	half = len / 2;
	a = arb_expand(a, len);
	b = arb_expand(b, len);
	a->len = a->lp = b->len = b->lp = len;

	*aa = arb_expand(NULL, half);
	*bb = arb_expand(NULL, half);
	*cc = arb_expand(NULL, half);
	*dd = arb_expand(NULL, half);
	_arb_copy_core((*aa)->number, a->number, half);
	_arb_copy_core((*bb)->number, a->number + half, half);
	_arb_copy_core((*cc)->number, b->number, half);
	_arb_copy_core((*dd)->number, b->number + half, half);

	(*aa)->len = (*aa)->lp = (*bb)->len = (*bb)->lp = half;
	(*cc)->len = (*cc)->lp = (*dd)->len = (*dd)->lp = half;

	/* return the total compensated magnitude */
	return compensated_mag;
}

void split_test(fxdpnt *a, fxdpnt *b)
{
	fxdpnt *aa = NULL;
	fxdpnt *bb = NULL;
	fxdpnt *cc = NULL;
	fxdpnt *dd = NULL;
	size_t comp = split(a, b, &aa, &bb, &cc, &dd);
	printf("compensated magnitude %zu\n", comp);
	arb_printtrue(a); 
	arb_printtrue(b);
	arb_printtrue(aa);
	arb_printtrue(bb);
	arb_printtrue(cc);
	arb_printtrue(dd);
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


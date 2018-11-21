#include "internal.h"

/*
	a normal split
        12345678 * 55559999

        1234*5555*10^8 + (1234*9999 + 5678*5555)*10^4 + 5678*9999

	for an odd split

	1235678 * 5559999
	123*555*10^8 + (123*9999 + 5678*555)*10^4 + 5678*9999


	for an uneven split

	1235678 * 55559999
	123*5555*10^8 + (123*9999 + 5678*5555)*10^4 + 5678*9999

	a dimishing uneven split requres smaller powers
	12345 * 55559999

	12*5555*10^7 + (12*9999 + 345*5555)*10^4 + 345*9999 


*/







size_t split(fxdpnt *a, fxdpnt *b, fxdpnt **aa, fxdpnt **bb, fxdpnt **cc, fxdpnt **dd)
{
	/* pass in two fxdpnts and four NULL new fxdpnts */
	size_t half = 0;
	half = a->len / 2;
	size_t compensated_mag = 0;
	size_t alen = a->len;
	size_t blen = b->len;
	size_t alentoadd = 0;
	size_t blentoadd = 0;
	size_t projected_len = 0;
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
		compensated_mag += 1;
	}

	half = len / 2;
	arb_expand(a, len);
	arb_expand(b, len);
	a->len = a->lp = b->len = b->lp = len;

	*aa = arb_expand(NULL, half);
	*bb = arb_expand(NULL, half);
	*cc = arb_expand(NULL, half);
	*dd = arb_expand(NULL, half);
	_arb_copy_core((*aa)->number, a->number, half);
	_arb_copy_core((*bb)->number, a->number + half, half);
	_arb_copy_core((*cc)->number, b->number, half);
	_arb_copy_core((*dd)->number, b->number + half, half);

	(*aa)->len = (*aa)->lp = (*bb)->len = (*bb)->lp = (*cc)->len = (*cc)->lp = (*dd)->len = (*dd)->lp = half;

	/* we need to check if a number is even or odd in order for a true split */
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
	arb_printtrue(a); 
	arb_printtrue(b);
	arb_printtrue(aa);
	arb_printtrue(bb);
	arb_printtrue(cc);
	arb_printtrue(dd);
}

fxdpnt *karatsuba(fxdpnt *a, fxdpnt *b, fxdpnt *c, int base, size_t scale)
{
	/* divide the number into halves */
	size_t ha = a->len / 2;
	size_t hb = b->len / 2;
	fxdpnt *front = arb_expand(NULL, ha + hb);
	fxdpnt *mid1 = arb_expand(NULL, ha + hb);
	fxdpnt *mid2 = arb_expand(NULL, ha + hb);
	fxdpnt *midtot = arb_expand(NULL,a->len + b->len);
	fxdpnt *end = arb_expand(NULL, ha + hb);
	fxdpnt *total = arb_expand(NULL, a->len + b->len);



	/* front half */
	front->lp=front->len= arb_mul_core(a->number, ha, b->number, hb, front->number, base);

	/* expand to the power of */
	arb_expand(front, a->len + b->len);
	front->lp = front->len = a->len + b->len;
	arb_print(front);
	/* sum into total */
	add(front, total, &total, base, 0);


	/* middle halves */
	mid1->lp=mid1->len= arb_mul_core(a->number, ha, b->number + hb, hb, mid1->number, base);
	arb_print(mid1);
	mid2->lp=mid2->len= arb_mul_core(a->number + ha, ha, b->number, hb, mid2->number, base);
	arb_print(mid2);

	/* sum middle halves */
	add(mid1, mid2, &midtot, base, 0);
	midtot->lp = midtot->len;

	/* expand to the power of */
	arb_expand(front, (ha + hb) * 2);
	midtot->lp = midtot->len = (ha + hb) + ((ha + hb)/2);
	/* sum into total */
	add(midtot, total, &total, base, 0);



	/* end halves */
	end->lp=end->len= arb_mul_core(a->number + ha, ha, b->number + hb, hb, end->number, base);
	arb_print(end);
	/* sum into total */
	add(end, total, &total, base, 0);
	

	

	return total;
}


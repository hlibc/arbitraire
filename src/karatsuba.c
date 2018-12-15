#include "internal.h"

/*
	a normal split
        12345678 * 55559999

        1234*5555*10^8 + (1234*9999 + 5678*5555)*10^4 + 5678*9999
*/

size_t split(fxdpnt *a, fxdpnt *b, fxdpnt **aa, fxdpnt **bb, fxdpnt **cc, fxdpnt **dd)
{
	/* pass in two fxdpnts and four NULL new fxdpnts */
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
	c = c;
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
	//size_t lpstore = a->lp + b->lp;
	total = arb_expand(NULL, a->len + b->len);
	size_t comp = split(a, b, &aa, &bb, &cc, &dd);
	/* front half */
	mul2(aa, cc, &front, base, scale, "total = ");
	/* expand to the power of */
	arb_expand(front, ((aa->len + bb->len) * 2));
	front->len = front->lp = ((aa->len + bb->len) * 2);
	arb_print(front);
	/* middle halves */
	mul2(aa, dd, &mid1, base, scale, "mid1 = ");
	arb_expand(mid1, (aa->len + bb->len + aa->len));
	mid1->len = mid1->lp = ((aa->len + bb->len + aa->len));
	mul2(bb, cc, &mid2, base, scale, "mid2 = ");
	arb_expand(mid2, (aa->len + bb->len + aa->len));
	mid2->len = mid2->lp = ((aa->len + bb->len + aa->len));
	/* sum middle halves */
	add(mid1, mid2, &midtot, base, "midtot = ");
	/* sum into total */
	add(midtot, front, &total, base, "total = ");
	/* end halves */
	mul2(bb, dd, &end, base, scale, "end = ");
	/* sum into total */
	add(end, total, &total, base, "total = ");

	total->lp = (total->len - comp);
	//total->lp = total->lp - (total->lp - lpstore);
	return total;
}


#include "internal.h"

/*
        12345678 * 55559999
        (1234*10^4 + 5678) * (5555*10^4 + 9999)

        1234*5555*10^8 + (1234*9999 + 5678*5555)*10^4 + 5678*9999
*/


fxdpnt *karatsuba(fxdpnt *a, fxdpnt *b, fxdpnt *c, int base, size_t scale)
{
	/* divide the number into halves */
	size_t halfa = a->len / 2;
	size_t halfb = b->len / 2;
	fxdpnt *front = arb_expand(NULL, halfa + halfb);
	fxdpnt *mid1 = arb_expand(NULL, halfa + halfb);
	fxdpnt *mid2 = arb_expand(NULL, halfa + halfb);
	fxdpnt *midtot = arb_expand(NULL,a->len + b->len);
	fxdpnt *end = arb_expand(NULL, halfa + halfb);
	fxdpnt *total = arb_expand(NULL, a->len + b->len);

	/* front half */
	front->len = arb_mul_core(a->number, halfa, b->number, halfb, front->number, base);
	front->lp = front->len;
	arb_print(front); 

	/* expand to the power of */
	arb_expand(front, a->len + b->len);
	front->len = a->len + b->len;
	front->lp = front->len;
	arb_print(front);

	/* middle halves */
	mid1->len = arb_mul_core(a->number, halfa, b->number+ halfb, halfb, mid1->number, base);
	mid1->lp = mid1->len;
	arb_print(mid1);
	
	mid2->len = arb_mul_core(a->number + halfa, halfa, b->number, halfb, mid2->number, base);
	mid2->lp = mid2->len;
	arb_print(mid2);

	/* sum middle halves */
	add(mid1, mid2, &midtot, base, 0);
	midtot->lp = midtot->len;

	/* expand to the power of */
	arb_expand(front, (halfa + halfb) * 2);
	midtot->len = (halfa + halfb) + ((halfa + halfb)/2);
	midtot->lp = midtot->len;
	arb_print(midtot); 

	/* end halves */
	end->len = arb_mul_core(a->number + halfa, halfa, b->number + halfb, halfb, end->number, base);
	end->lp = end->len;
	arb_print(end);

	/* sum the parts */
	add(front, total, &total, base, 0);
	add(midtot, total, &total, base, 0);
	add(end, total, &total, base, 0);
	
	return total;
}

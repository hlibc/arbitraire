#include "internal.h"

void _arb_copyreverse_core(UARBT *b, UARBT *a, size_t len)
{
	size_t i = 0;
	size_t j = len - 1;
	for (i=0;i < len;++i, j--)
		b[i] = a[j];
}

void arb_copyreverse(fxdpnt *b, fxdpnt *a)
{ 
	b = arb_expand(b, a->len);
	b->len = a->len;
	b->lp = a->lp;
	b->sign = a->sign;
	_arb_copyreverse_core(b->number, a->number, a->len);
}

void _arb_copy_core(UARBT *b, UARBT *a, size_t len)
{
	memcpy(b, a, len * sizeof(UARBT));
}

void *arb_copy(fxdpnt *b, fxdpnt *a)
{ 
	b = arb_expand(b, a->len);
	b->len = a->len;
	b->lp = a->lp;
	b->sign = a->sign;
	_arb_copy_core(b->number, a->number, a->len);
	return b;
}


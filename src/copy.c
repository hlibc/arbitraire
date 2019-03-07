#include "internal.h"

/* Copyright 2017-2019 CM Graff */


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


#include "internal.h"

/* Copyright 2017-2019 CM Graff */

fxdpnt *remove_leading_zeros(fxdpnt *c)
{
	int effect = 0;
	size_t i = 0;
	
	for (i=0;c->number[i] == 0 && (c->lp > 1 || (c->lp > 0 && rr(c)));) { 
		c->lp--;
		++i;
		effect = 1;
	}

	if (effect) {
		c = arb_leftshift(c, i);
		c->len -= i;
	}
	return c;
}

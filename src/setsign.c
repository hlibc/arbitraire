#include "internal.h"

/*
 * Copyright 2017-2019 CM Graff
 */

/* Because arb_setsign calls arb_init, functions that use arb_setsign
 * don't need to also use arb_init
 */

void arb_setsign(const fxdpnt *a, const fxdpnt *b, fxdpnt *c)
{
	arb_init(c);
	if (a->sign == '-')
		arb_flipsign(c);
	if (b->sign == '-')
		arb_flipsign(c);
}

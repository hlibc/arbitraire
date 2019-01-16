#include "internal.h"

void arb_setsign(const fxdpnt *a, const fxdpnt *b, fxdpnt *c)
{
	arb_init(c);
	if (a->sign == '-')
		arb_flipsign(c);
	if (b->sign == '-')
		arb_flipsign(c);
}

#include "internal.h"

void arb_flipsign(fxdpnt *flt)
{
	if (flt->sign == '+')
		flt->sign = '-';
	else if (flt->sign == '-')
		flt->sign = '+';
}


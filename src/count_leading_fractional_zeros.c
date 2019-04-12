#include "internal.h"


size_t count_leading_fractional_zeros(fxdpnt *a)
{
	size_t pos = a->len - (a->len - a->lp);
	while (a->number[pos] == 0)
	{
		pos++;
	}
	return pos;
}



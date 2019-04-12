#include "internal.h"


size_t count_leading_fractional_zeros(fxdpnt *a)
{
	size_t pos = a->len - (a->len - a->lp);
	size_t ret = 0;
	while (a->number[pos] == 0)
	{
		pos++;
		ret++;
	}
	return ret;
}



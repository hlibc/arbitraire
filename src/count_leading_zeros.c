#include "internal.h"


size_t count_leading_zeros(const fxdpnt *a)
{
	size_t len = a->lp;
	size_t ret = 0;
	if (a->lp > 0) {
		while (a->number[ret] == 0 && ret < len)
			++ret;
	}
	return ret;
}



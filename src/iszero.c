#include "internal.h"

int iszero(fxdpnt *a)
{
	size_t i = 0;
	for (i=0; i < a->len; ++i) {
		if (a->number[i] != 0)
			return 1;
	}
	return 0;
}

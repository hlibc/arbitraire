#include "internal.h"

/* Copyright 2017-2019 CM Graff */


int iszero(const fxdpnt *a)
{
	size_t i = 0;
	for (i=0; i < a->len; ++i) {
		if (a->number[i] != 0)
			return 1;
	}
	return 0;
}


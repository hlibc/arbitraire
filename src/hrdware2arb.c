#include "internal.h"

/* Copyright 2017-2019 CM Graff */

fxdpnt *hrdware2arb(size_t a)
{
	char str[50];
	sprintf(str, "%zu", a);
	fxdpnt *ret = arb_str2fxdpnt(str);
	return ret;
}


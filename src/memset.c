#include "internal.h"

/* Copyright 2017-2019 CM Graff */


void *_arb_memset(void *s, int c, size_t n)
{
	return memset(s, c, n * sizeof(UARBT));
}


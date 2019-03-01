#include "internal.h"

void *_arb_memset(void *s, int c, size_t n)
{
	return memset(s, c, n * sizeof(UARBT));
}


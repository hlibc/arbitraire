#include "internal.h"
/*
 * Copyright 2017-2019 CM Graff
 */

int oddity(size_t len)
{
	/* return true if odd, return zero if even */
	if (len % 2 == 1) {
                return 1;
        }
	return 0;
}

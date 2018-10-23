#include <stdlib.h>
#include <limits.h>

/*
 * a rem b with division
 * - returns LONG_MAX if b is zero
 */
long hrdware_div_rem(long a, long b)
{
	long diff = labs(a) - labs(b);
	
	if (b) {
		if (diff > 0) {
			long val = b / a;
			val *= a;
			a -= val;
		}

		return a;
	}

	return LONG_MAX;
}


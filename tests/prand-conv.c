#include <arbitraire/arbitraire.h>
#include <stdlib.h>
#include <time.h>
#define MAXIMA 10000
#include "make_bignum.h"

int main(void)
{ 
	char *string1 = make_bignum(MAXIMA, 10, 1);
	fxdpnt *a;
	a = arb_str2fxdpnt(string1);
	fxdpnt *b = arb_expand(NULL, arb_size(a));

	int base = 0;
	while ( base <2 || (base > 10 && base < 16))
		base = random() % 16;

	fprintf(stderr, "ibase=10;obase=%d;%s+0\nquit\n", base, string1);
	a = convscaled(a, b, 10, base);
	
	arb_print(b);
	arb_free(a);


	return 0;
}


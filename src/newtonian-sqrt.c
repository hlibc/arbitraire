#include "internal.h"

/*
	Newton's swuare root is a typical algorithm which is more or less
	identical to the Babylonian Method. We deploy a variation which tests
	the scales of the two guesses and allows the formula to break out of
	the loop, ergo avoiding oscillation and other pitfalls.

	This method always produces correct answers as it is scaled past the
	needed precision and it never siezes up in an infinite loop as it has
	a mechanism for breaking out of the loop based on the scale of the
	converging "guesses".
*/

fxdpnt *nsqrt(fxdpnt *a, int base, size_t scale)
{
	size_t s1 = 0;
	size_t s2 = 2;
	
	fxdpnt *g = arb_str2fxdpnt("10");
	fxdpnt *g1 = arb_expand(NULL, (a)->len);
	

	if (a->sign == '-')
		return NULL;
	
	if ((a->lp)<2){
		arb_copy(g, one);
	}
	else
	{
		arb_free(g1);
		g1 = hrdware2arb((a)->lp);
		g1 = arb_mul(g1, p5, g1, base, 0);
		g = arb_exp(g, g1, g, base, 0);
	}
	
	for(s1 = MAX(rr(a), scale);;)
	{
		arb_copy(g1, g);
		g = arb_div(a, g, g, base, s1);
		g = arb_add(g, g1, g, base);
		g = arb_mul(g, p5, g, base, s1);
		if ((arb_compare(g, g1, base) == 0))
		{
			if (s2 < s1+1)
				s2 = MIN(s2*3, s1+1);
			else
				break;
		}
	}
	a = arb_div(g, one, a, base, s1);
	arb_free(g);
	arb_free(g1);
	return a;
}


#include "internal.h"

/*
         1         A
a[i+1] = - * ( --------- + (n-1)*a[i] )
         n     a[i]^(n-1)

*/


fxdpnt *nthroot(fxdpnt *a, int base, size_t scale)
{
	size_t s1 = 0;
	size_t s2 = 2;
	
	fxdpnt *g = arb_str2fxdpnt("1.000");
	fxdpnt *g1 = arb_expand(NULL, (a)->len);

	fxdpnt *N = arb_str2fxdpnt("6.00");
	fxdpnt *p3 = arb_str2fxdpnt(".3333333");
	

	if (a->sign == '-')
		return NULL;
	
	//if ((a->lp)<2){
		arb_copy(g, N);
	
///	}
	/*
	else
	{
		printf("here\n");
		arb_free(g1);
		g1 = hrdware2arb((a)->lp);
		//g1 = arb_mul(g1, p5, g1, base, 0);
		printf("above div\n");
		g1 = arb_div(g1, N, g1, base, 0);
		printf("above exp\n");
		g = arb_exp(g, g1, g, base, 0);
	}
	*/
	printf("high\n");
	for(s1 = MAX(rr(a), scale);;)
	{
		arb_copy(g1, g);
		g = arb_div(a, g, g, base, s1);
		g = arb_add(g, g1, g, base);
		g = arb_div(g, N, g, base, s1);
		if ((arb_compare(g, g1, base) == 0))
		{
			//if (s2 < s1+1)
				//s2 = MIN(s2*3, s1+1);
			//	s2 = MIN(s2*5, s1+1);
			//else
				break;
		}
		arb_print(g1);
	}
	//a = arb_div(g, one, a, base, s1);
	a = arb_div(g, N, a, base, s1);
	arb_free(g);
	arb_free(g1);
	return a;
}


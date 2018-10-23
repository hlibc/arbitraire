#include <arbitraire/arbitraire.h>

void arb_freeglobals(void)
{
	arb_free(zero);
	arb_free(p5);
	arb_free(one);
	arb_free(two);
	arb_free(ten);
}

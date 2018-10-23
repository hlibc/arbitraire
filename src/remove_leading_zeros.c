#include <arbitraire/arbitraire.h>

fxdpnt *remove_leading_zeros(fxdpnt *c)
{
	int effect = 0;
	size_t i = 0;
	
	for (i=0;c->number[i] == 0 && (c->lp > 1 || (c->lp > 0 && rr(c)));)
	{ 
		c->lp--;
		++i;
		effect = 1;
	}

	if (effect)
	{
		c = arb_leftshift(c, i, 1);
		c->len -= i;
	}
	return c;
}

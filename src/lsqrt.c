#include "internal.h"
/*
	This function is under construction
*/

static fxdpnt *factor(fxdpnt *a, fxdpnt *b, int base, size_t scale)
{
	/* regular factorization. we only need to obtain two
		digit numbers so the naive algorithm is fine
	*/
	fxdpnt *temp = arb_str2fxdpnt("+0.00");
	arb_copy(temp, a);
	int comp = -100;
	do
	{
		mul(a, a, &temp, base, scale, 0);
		comp = arb_compare(temp, b,  10);
		if (comp == 0) {
			break;
		} else if (comp > 0)
		{
			decr(&a, base, 0);
			break;
		}
		incr(&a, base, 0);
	}while (1);
	return a;
}

void factor2(fxdpnt **a, fxdpnt *b, int base, size_t scale)
{
	*a = factor(*a, b, base, scale);
}

static fxdpnt *guess(fxdpnt **c, fxdpnt *b, int base, size_t scale, char *m)
{
	/* Handle sqrt factorization guesses of the form
		465n * n < guess
		return the small guess
		populate the large number as "c"
	*/
	_internal_debug;
	fxdpnt *side = arb_str2fxdpnt("1");
	fxdpnt *tmul = arb_str2fxdpnt("1");
	int comp = -100;
	do
	{ 
		mul(*c, side, &tmul, base, scale, 0);
		comp = arb_compare(tmul, b, 10);
		if (comp == 0) {
			break;
		} else if (comp > 0)
		{
			decr(&*c, base, 0);
			decr(&side, base, 0);
			break;
		}
		incr(&side, base, 0);
		incr(&*c, base, 0);
	}while(1);
	_internal_debug_end;
	return side;
}

void pushon(fxdpnt *c, fxdpnt *b)
{
	arb_expand(c, c->len + b->len);
	_arb_copy_core(c->number + c->len, b->number, b->len);
	c->len += b->len;
	c->lp = c->len;
	
}
void push(fxdpnt **c, fxdpnt *b, char *m)
{
	_internal_debug;
	pushon(*c, b);
	_internal_debug_end;
}
void addfront(fxdpnt *a, fxdpnt *b)
{
	arb_expand(a, b->len);
	_arb_copy_core(a->number, b->number, b->len);
}

static void cap(fxdpnt **c, fxdpnt *b, char *m)
{
	_internal_debug;
	addfront(*c, b);
	_internal_debug_end;
}

fxdpnt *grabdigits(fxdpnt *digi, fxdpnt *a, size_t *gotten, size_t digits_to_get)
{ 
	if (*gotten + digits_to_get >= a->len)
	{
		arb_expand(digi, digi->len + *gotten + digits_to_get);
		_arb_memset(digi->number + digi->len, 0, digits_to_get);
	} else {
		_arb_copy_core(digi->number, a->number + *gotten, digits_to_get);
	}
	digi->lp += digits_to_get;
	digi->len += digits_to_get;
	*gotten += digits_to_get;
	return digi;
}
fxdpnt *long_sqrt(fxdpnt *a, int base, size_t scale)
{
	
	int digits_to_get = 2;
	size_t gotten = 0;
	size_t i = 0;
	fxdpnt *digi = arb_str2fxdpnt("");  //arb_expand(NULL, a->len);
	fxdpnt *g1 = arb_str2fxdpnt("");
	fxdpnt *g2 = arb_str2fxdpnt("");
	fxdpnt *ans = arb_str2fxdpnt("");
	fxdpnt *fac = arb_str2fxdpnt("");
	fxdpnt *side = arb_str2fxdpnt("");
	fxdpnt *temp = arb_str2fxdpnt("");
	arb_copy(g1, a);
	arb_copy(g2, a);
	_arb_memset(g1->number, 0, g1->len);
	
	
	//if (oddity(rl(a))) {
	//	digits_to_get = 2;
	//} else
	if (oddity(a->lp)) {
		digits_to_get = 1;
	}
	

	/* get the first guess */
	digi = grabdigits(digi, a, &gotten, digits_to_get);
	digits_to_get = 2; 
	factor2(&fac, digi, base, scale);
	push(&ans, fac, "ans = ");
	mul(ans, ans, &temp, base, scale, "temp = ");
	cap(&g1, temp, "g1 = "); 

	/* now try to subtract the first guess from the input */
	mul(ans, two, &side, base, scale, "side = "); 
	push(&side, one, "side = ");
	sub(g2, g1, &g2, base, "g2 = "); 
	//g2 = grabdigits(g2, a, &gotten, digits_to_get); 
	digi = guess(&side, g2, base, scale, "side = "); 
	push(&ans, digi, "ans = "); 
	mul(side, digi, &g1, base, scale, "g1 = ");
	
	
	top:
	
	mul(ans, two, &side, base, scale, "side = "); 
	push(&side, one, "side = ");
	sub(g2, g1, &g2, base, "g1 = "); 
	g2 = grabdigits(g2, a, &gotten, digits_to_get); 
	digi = guess(&side, g2, base, scale, "side = "); 
	push(&ans, digi, "ans = "); 
	mul(side, digi, &g1, base, scale, "g1 = "); 

	while (i++ < scale - 1)
	goto top;
	
	return ans;
}

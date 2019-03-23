#include "internal.h"
/*
	This function is under construction

	
	square root of 31.50:

	            _5.612 ..._
	           /31.50 00 00 00 00 00 00 00
	           -25
	           ---
	             6 50
	  10_6 * 6 = 6 36
	             ----
		       14 00
	   112_1 * 1 = 11 21
	               -----
		        2 79 00
	   1122_2 * 2 = 2 24 44
	                -------
	                  54 56 00
	    11224_N * N = ...


	square root of 3131

	        _55.9 ..._
	       /31 31.00 00 00 00
	       -25
	        --
	         6 31
	10_5*5 = 5 25
	         ----
	         1 06 00
	110_9*9=   99 81
	           -----
	            6 19 00

	1118_N*N= 


	square root of 283.6

	         _16.8 ..._
	        /2 83.60 00 00 00 00
	        -1
	         ----
	         1 83
	 2_6*6 = 1 56
	         ----
	           27 60
	32_8*8 =   26 24
	           -----
	            1 36 00


	square root 99 and 999

	           _9. 9 ... _
	          /99.00
	           81
	           ---
	           18 00
	18_9 * 9 = 17 81


	         _31. ..._
	        /999
	         9
	         -
	         099
	9_1 * 1 = 91
	         ---
	          8

	TODO:	* add a sequence that forces a "side zero"
		* test the difference in performance between multiplying
		  a number by two versus adding it to itself.

	Square root has two different sequences depending on the number of digits
	to the left of the radix. For instance '99' and '999' produce totally
	different output, but '9', '999' and '99999' all produce sinilar sequences.
	This behavior appears to arise from B1B2 being chosen for the initial 
	decomposition as opposed to B1. 
*/

static fxdpnt *factor(fxdpnt *a, fxdpnt *b, int base, size_t scale)
{
	/* regular factorization. we only need to obtain two
	   digit numbers
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

static void factor2(fxdpnt **a, fxdpnt *b, int base, size_t scale)
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

static void pushon(fxdpnt *c, fxdpnt *b)
{
	c = arb_expand(c, c->len + b->len);
	_arb_copy_core(c->number + c->len, b->number, b->len);
	c->len += b->len;
	c->lp = c->len;
}

static void push(fxdpnt **c, fxdpnt *b, char *m)
{
	_internal_debug;
	pushon(*c, b);
	_internal_debug_end;
}

static void addfront(fxdpnt *a, fxdpnt *b)
{
	a = arb_expand(a, b->len);
	_arb_copy_core(a->number, b->number, b->len);
}

static void cap(fxdpnt **c, fxdpnt *b, char *m)
{
	_internal_debug;
	addfront(*c, b);
	_internal_debug_end;
}

static fxdpnt *grabdigits(fxdpnt *digi, fxdpnt *a, size_t *gotten, size_t digits_to_get)
{ 
	if (*gotten + digits_to_get >= a->len)
	{
		digi = arb_expand(digi, digi->len + *gotten + digits_to_get);
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
	g1 = arb_copy(g1, a);
	g2 = arb_copy(g2, a);
	_arb_memset(g1->number, 0, g1->len);

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

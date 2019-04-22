#include"internal.h"
/*

	Copyright 2019 CM Graff. All Rights Reserved

	This is an arbitrary precision implementation of the long-hand square 
	root algorithm.

	square root of 31.50:

	            _5.612 ..._
	          \/31.50 00 00 00 00 00 00 00
	           -25
	           ---
	             6 50
	10_6*6=      6 36
	             ----
		       14 00
	112_1*1=       11 21
	               -----
		        2 79 00
	1122_2*2=       2 24 44
	                -------
	                  54 56 00
	11224_N*N= ...


	square root of 3131

	        _55.9 ..._
	      \/31 31.00 00 00 00
	       -25
	        --
	         6 31
	10_5*5 = 5 25
	         ----
	         1 06 00
	110_9*9=   99 81
	           -----
	            6 19 00 
	1118_N*N= ...


	square root of 283.6

	         _16.8 ..._
	       \/2 83.60 00 00 00 00
	        -1
	         ----
	         1 83
	2_6*6=   1 56
	         ----
	           27 60
	32_8*8=    26 24
	           -----
	            1 36 00


	square root 99 and 999

	           _9. 9 ..._
	         \/99.00
	           81
	           ---
	           18 00
	18_9 * 9 = 17 81


	         _31. ..._
	       \/999
	         9
	         -
	         099
	9_1 * 1 = 91
	         ---
	          8


	square root of 283.6

	     __1__6.8_4_0_4_..._
	   \/  2 83.6
	       1
	       -----
	       1 83
	2_6*6= 1 56
	       ------
	         27 60
	32_8*8=  26 24
	        -------	
	          1 36 00
	336_4*4=  1 34 56
	          --------
	             1 44 00
	3368_0*0=       0 00   <-- Note that the side guess can be a zero
	           ---------
	             1 44 00 00
	33680_4*4=   1 34 72 16
	             ----------
		     ...

*/

static fxdpnt *factor(fxdpnt *a, fxdpnt *b, int base, size_t scale)
{
	/* regular factorization. we only need to obtain two
	   digit numbers
	   TODO: make this function return the squared number 
	   so we can save a multiplication later
	*/
	fxdpnt *temp = arb_str2fxdpnt("+0.00");
	temp = arb_copy(temp, a);
	int comp = -100;
	do
	{
		mul(a, a, &temp, base, scale, 0);
		comp = arb_compare(temp, b);
		if (comp == 0) {
			break;
		} else if (comp > 0) {
			decr(&a, base, 0);
			break;
		}
		incr(&a, base, 0);
	}while (1);
	arb_free(temp);
	return a;
}

/* factor2() and push2() are convenience wrappers */
static void factor2(fxdpnt **a, fxdpnt *b, int base, size_t scale)
{
	*a = factor(*a, b, base, scale);
}

static fxdpnt *push(fxdpnt *c, fxdpnt *b)
{
	c = arb_expand(c, c->len + b->len);
	_arb_copy_core(c->number + c->len, b->number, b->len);
	c->len += b->len;
	c->lp = c->len;
	return c;
}

static void push2(fxdpnt **c, fxdpnt *b, char *m)
{
	_internal_debug;
	*c = push(*c, b);
	_internal_debug_end;
}

static fxdpnt *guess(fxdpnt **c, fxdpnt *b, int base, size_t scale, char *m)
{
	/* Handle sqrt factorization guesses of the form:
		465n * n < guess
		return the small guess
		populate the large number as "c"
		TODO: reuse the multiplication that this produces in tmul
			  -- this is a bit hard as we depend on overflowing
			  so we'd need to use two variables as a kind of cache
		TODO: make a better first guess
			Hypothesis:

			start with base / 2 instead of 1, and work up or down
			from there, divide repeatedly until the comparisons shift
			state

			if base / 2 is too large, then halve it again. if it is too
			small then add base / 4 to it

			once the state shift occurs, then increment or decrement

			the method currently used is fine for small bases, but 
			may be slow for large bases.
	*/
	_internal_debug;
	fxdpnt *side = arb_str2fxdpnt("1");
	fxdpnt *tmul = arb_str2fxdpnt("1");
	int comp = -100;
	do
	{
		mul(*c, side, &tmul, base, scale, 0);
		comp = arb_compare(tmul, b);
		if (comp == 0) {
			break;
		} else if (comp > 0) {
			decr(&*c, base, 0);
			decr(&side, base, 0);
			break;
		}
		incr(&side, base, 0);
		incr(&*c, base, 0);
	}while(1);
	arb_free(tmul);
	_internal_debug_end;
	return side;
}

fxdpnt *lhsqrt(fxdpnt *aa, int base, size_t scale)
{
	/* TODO: oddity of the log() of the fractional part is not a valid
	 * concept. Instead, track the leading fractional zeros.
	 * TODO: numbers without fractional parts are failing.
	 */
	int dig2get = 2;
	size_t i = 0;
	int firstpass = 1;
	int odd = 0;
	int lodd = 0;
	fxdpnt *a = NULL;
	a = arb_copy(a, aa);
	a = remove_leading_zeros(a);
	size_t suppl = MAX((scale*2), rr(a) * 2);
	//size_t suppl = MAX(scale, rr(a));
	fxdpnt *g1 = arb_expand(NULL, a->len);
	fxdpnt *t = NULL;
	fxdpnt *answer = arb_expand(NULL, a->len + scale);

	fxdpnt *side = arb_expand(NULL, a->len + scale);
	arb_init(answer);
	fxdpnt *g2 = NULL;

	fxdpnt *x1 = arb_expand(NULL, a->len);
	fxdpnt *tmp = x1;
	UARBT *f = tmp->number;

	if (oddity(a->lp)) {
		dig2get = 1;
		lodd = 1;
	}

	if (oddity(rr(a))) {
		odd = 1;
	}

	size_t zeros = count_leading_fractional_zeros(a);

	if (zeros) {
		i = zeros;
		if (oddity(zeros))
			dig2get = 1;
	}

	for (;i < a->len + odd + suppl; ) {
		/* distribute blocks of numbers */
		if (i == a->len -1) {
			x1 = tmp;
			x1->number[0] = a->number[i];
			x1->number[1] = 0;
			x1->lp = x1->len = dig2get;
		} else if (i > a->len -1) {
			x1 = tmp;
			x1->number[0] = x1->number[1] = 0;
			x1->lp = x1->len = dig2get;
		}
		else if (i < a->len -1) {
			x1->number = a->number + i;
			x1->lp = x1->len = dig2get;
		}
		// TODO: separate out this conditional
		if (firstpass) {
			factor2(&g1, x1, base, scale);
			push2(&answer, g1, "answer = ");
			mul(g1, g1, &g1, base, scale, "g1 = ");
			sub(x1, g1, &g1, base, "g1 = ");
			firstpass = 0;
		}else {
			push2(&g1, x1, "g1 = ");
			/* mul by 2, append, and then factor up */
			//mul(answer, two, &side, base, scale, "side = ");
			add(answer, answer, &side, base, 0);
			push2(&side, one, "side = ");
			t = guess(&side, g1, base, scale, "side = ");
			mul(t, side, &g2, base, scale, "g2 =");
			push2(&answer, t, "answer = ");
			arb_free(t);
			sub(g1, g2, &g1, base, "g1 = ");
		}
		
		i += dig2get;
		dig2get = 2;
	}

	arb_free(g1);
	arb_free(g2);
	arb_free(side);
	tmp->number = f;
	arb_free(tmp);
	answer = arb_rightshift(answer, zeros / 2);
	answer->lp = a->lp / 2 + lodd;
	answer->len = answer->lp + MAX(scale, rr(a));
	arb_free(a);
	
	return answer;
} 




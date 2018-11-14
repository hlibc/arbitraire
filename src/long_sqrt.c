#include "internal.h"
/*

	https://gmplib.org/manual/Square-Root-Algorithm.html


	This function is under construction.


               75.29
            ________
          \/ 5669.00
             49
             _______
7*20 = 145  | 769
         -    725
             _______
75*20= 1502 |  44 00
          -    30 04
              _______
752*20=15049 | 13 9600
           -   13 5441
                _________
7529*20=150582 |  315900
             -

            23.8
          _______
        \/5 69.00
          4
         _______       44 * 4 = 176
     43 | 1 69         43 * 3 = 129
          1 29
          ______       469 * 9 = 4221
     468 |  40.00      468 * 8 = 3744
            37.44

        * Get the first number set
        * attempt to find the square of the number set using multiplication
        * push the discovered number onto the answer
        * perform a subtraction and carry down 2 digits from the number that is being squared
        * multiply the answer by 2 and put into temp
        * find how many times temp can go into the carried down sub and its two digits


	* if the number has an even length you can get two digits but if the length
	 *  is odd then you'll need to start with one (though 3 may work??)

	
	There is no position agnostic way to generalize long hand
	square root

	for instance if you started with sqrt(5555)
	and tried to factor the first 5, you would get a "2"
	and this is incorrect because the sqrt of 5555 is
	74.531..., so you need to first start with the "55" in 
	order to derive a "7" which is the first correct digit

	1> factor the leading set
	2> square the initial factorization and carry it down
	3> subtract the new guess from the primary
	4> multiply the entire estimated answer by 2 and put it to the side
	5> factor the side new guess up the last guess by manipulating
	   its final digit (this should be seamless with factor())
	6> take the last digit of the side new guess and push it
		onto the answer, then use that guessing factor and replace
		the primary guess
	7> carry down two digits
	8> subtract the new guess from the old guess
	9> repeat

	notes:
		instead of pushing the answer digits onto a stack we
	can use a pure maths method of multiplying by 10 (base) and
	then adding the number. this will be an expensive procedure
	but it can be optimized away at a later time

	"a' can not be mutated. it needs to stay the same so
	we can pull digits from it. However, when it runs out of
	digits we need to populate the new guess using a fake array
	of zeros

	ergo we need (at least) the following variables
		answer
 	    _________________
	 \ /  a (input)
side          guess1
digit         sum
              guess1
              sum

	side * digi is computed to get a "digi" to push onto
	the "answer". but it is also computed in order to get the 
	next "guess". thi snew guess must then be subtracted from
	the "sum" to get a new "sum"

	answer is squared to get "side"

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
	memcpy(c->number + c->len, b->number, b->len * sizeof(ARBT));
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
	memcpy(a->number, b->number, b->len * sizeof(ARBT));
}

static void cap(fxdpnt **c, fxdpnt *b, char *m)
{
	_internal_debug;
	addfront(*c, b);
	_internal_debug_end;
}

void grabdigits(fxdpnt *digi, fxdpnt *a, size_t *gotten, size_t digits_to_get)
{ 
	memcpy(digi->number, a->number + *gotten, digits_to_get);
        digi->lp += digits_to_get;
        digi->len += digits_to_get;
        *gotten += digits_to_get;
}
fxdpnt *long_sqrt(fxdpnt *a, int base, size_t scale)
{
	a = a;
	base = base;
	scale = scale;
	int odd = 0;
	int digits_to_get = 2;
	size_t gotten = 0;
	fxdpnt *digi = arb_str2fxdpnt("");  //arb_expand(NULL, a->len);
	fxdpnt *g1 = arb_str2fxdpnt("");
	fxdpnt *ans = arb_str2fxdpnt("");
	fxdpnt *fac = arb_str2fxdpnt("");
	fxdpnt *side = arb_str2fxdpnt("");
	fxdpnt *subtract = arb_str2fxdpnt("");
	arb_copy(subtract, a);
	memset(subtract->number, 0, subtract->len);
	
	if (a->lp % 2 == 1) {
		odd = 1;
		digits_to_get = 1;
	}

	/* get first set of digits */
	grabdigits(digi, a, &gotten, digits_to_get);
	digits_to_get = 2;

	/* now factorize up to those one or two digits */
	factor2(&fac, digi, base, scale);
	push(&ans, fac, "ans = ");

	/*  square the ans */
	mul(ans, ans, &g1, base, scale, "g1 = ");
	int lever = 1;
	printf("intialized vvvvvvvvv\n");
	top:


	/* mul the ans by two */
	mul(ans, two, &side, base, scale, "side = "); 

	/* now push a one onto the "side" */
	push(&side, one, "side = "); 

	/* now subtract the guess 1 from the original */
	if (lever--)
	cap(&subtract, g1, "subtract = "); 
	sub(a, subtract, &a, base, "a = "); 

	/* now try to factorize the side guess up to the new original */ 
	digi = guess(&side, a, base, scale, "side = "); 

	/* push the new digi onto the answer */
	push(&ans, digi, "ans = ");

	/* mul side by digi to obtain the new "subtract" */
	mul(side, digi, &subtract, base, scale, "subtract = ");

	/* pull down two digits onto the new answer */
	//if (gotten >= a->len) else
		grabdigits(digi, a, &gotten, digits_to_get);
	arb_print(a);


	printf("============================\n");
	static size_t i = 4;
	if (i--)
	goto top;

	printf("bogus ans = ");

	return ans;
}

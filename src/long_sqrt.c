#include "internal.h"
/*
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

        while (1)
        {
                if ( num * num == num1 )
                {
                        break;
                } else if (num * num > num1 ) {
                        --num;
                        break;
                }
                ++num;


        }

	this style of loop should work for figuring out the squares

	
	There is no position agnostic way to generalize long hand
	square root

	for instance if you started with sqrt(5555)
	and tried to factor the first 5, you would get a "2"
	and this is incorrect because the sqrt of 5555 is
	74.531..., so you need to first start with the "55" in 
	order to derive a "7" which is the first correct digit

	1> factor the leading set
	2> square the initial factoziation and carry it down
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

*/

fxdpnt *factor(fxdpnt *a, fxdpnt *b, int base, size_t scale)
{
	/* factorization can be massivly sped up by handling
	   the number logically.
		xxx*xxx is [10000, bbbbbb) and sigma_{n<bbbbbb}(y+1) 
		where b is base-1 and y is 10000
		
		This method can be deployed with the following code:
		a = arb_expand(a, b->len / 2);
		a->number[0] = 1;
		a->lp = a->len;
	*/
	fxdpnt *temp = arb_str2fxdpnt("+0.00");
	arb_copy(temp, a);
	int comp = -100;
	size_t i = 0;
	while (1)
	{
		temp = arb_mul(a, a, temp, base, scale);
		comp = arb_compare(temp, b,  10);
		if (comp == 0) {
			break;
		} else if (comp > 0)
		{
			arb_sub2(a, one, &a, base);
			break;
		}
		arb_incr(&a, base);
		++i;
	}
	printf("iterations to factor %zu\n", i);
	return a;
}

fxdpnt *endfactor(fxdpnt *a, int base, size_t scale)
{
	
}
fxdpnt *long_sqrt(fxdpnt *a, int base, size_t scale)
{
	a = a;
	base = base;
	scale = scale;
	int odd = 0;
	int digits_to_get = 2;
	fxdpnt *start = arb_str2fxdpnt("2.0");
	fxdpnt *digi = arb_expand(NULL, a->len);
	size_t gotten = 0;
	if (a->lp % 2 == 1)
	{
		odd = 1;
		digits_to_get = 1;
	}
	/* get first set of digits */
	memcpy(digi->number, a->number + gotten, digits_to_get);
	digi->lp = 0;
	digi->len = digits_to_get;
	gotten += digits_to_get;
	digits_to_get = 2;

	printf("digi: ");
	arb_print(digi);
	/* now factorize up to those two digits */
	

	

	if (odd)
		printf("number was odd -- get 1 digit\n");
	else
		printf("number was even -- get 2 digits\n");

	start = factor(start, a, base, scale);
	
	
	return start;
}

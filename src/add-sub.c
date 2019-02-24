#include "internal.h"

/*
	These addition and subtraction routines are based on a type of
	imaginary array of zeros. This imaginary array is called upon
	to make up for the difference needed in order to perform the
	operations on numbers of differing magnitude.

	While many subtraction routines require that the numbers first
	be compared and then rearranged in the case that the zero threshold
	is crossed, we mitigate this by using a special property of
	subtraction that takes advantage of a left over carry. When a
	left over carry is detected, the inverse of the solution is used.
	Ergo, if the solution was 999 we would perform the operation:

		answer = 1000 - 999

	Instead of performing this operation in its entirety however, the
	inverse solution is calculated alongside the normative one and
	simply discarded in the case it is not needed (when there is no left
	over carry).
*/

/* _pl() draws from an imaginary array of zeros and allows add and sub to work
   on numbers with varying magnitudes in a concise manner
*/

/*
	todo: alternate add and sub impls

	most implementations are using compare() beforehand for subtractions

	using this technique it is probably not required to do the comparison
	for addition itself
*/
fxdpnt *newadd(const fxdpnt *a, const fxdpnt *b, fxdpnt *c, int base)
{
	/* 
	This is a new experimental add() function that should be a little
	faster than arb_add()
	The lower while loop needs to have the conditionals moved out
	in order for the design of newadd to be complete.
	*/
	size_t i = 0;
	size_t j = MAX(rr(a), rr(b)) + MAX(rl(a), rl(b));
	size_t len = 0;
	int sum = 0;
	int carry = 0;
	size_t k = 0;
	size_t z = a->len;
	size_t y = b->len;
	size_t hold = j;

	/* take care of differing tails to the right of the radix */
	if (rr(a) > rr(b))
	{
		len = rr(a) - rr(b);
		for (i=0;i<len;i++, j--, z--)
		{
			c->number[j] = a->number[z];
		}
	}
	
	else if (rr(b) > rr(a))
	{
		len = rr(b) - rr(a);
		for (k=0;k<len;k++, j--, y--)
		{
			c->number[j] = b->number[y];
		}
	}
	/* numbers are now compatible for a straight-forward add */
	for (;i<=a->len || k <= b->len;i++, j--, k++)
	{
		if (i <= le(a) && k <= le(b))
			sum = a->number[z--] + b->number[y--] + carry;
		else if (i <= le(a))
			sum = a->number[z--] + carry;
		else if (k <= le(b))
			sum = b->number[y--] + carry;
			
		carry = 0;
		if (sum >= base)
		{
			sum -= base;
			carry = 1;
		}
		c->number[j] = sum;
	}
	
	c->len = hold;
	if (carry) {
		for(i = c->len+1;i > 0; i--)
			c->number[i] = c->number[i-1];
		c->number[0] = 1;
		c->len++;
		c->lp++;
	}

	return c;
}

static UARBT _pl(const fxdpnt *a, const fxdpnt *b, size_t *cnt, size_t r)
{
	UARBT temp = 0;
	if ((rr(a)) < (rr(b)))
		if((rr(b)) - (rr(a)) > r)
			return 0;

	if (*cnt < a->len){
		temp = a->number[a->len - *cnt - 1];
		(*cnt)++;
	}
	return temp;
}

/* the actual addition */
fxdpnt *arb_add_inter(const fxdpnt *a, const fxdpnt *b, fxdpnt *c, int base)
{
	size_t i = 0;
	size_t j = 0;
	ARBT sum = 0;
	uint8_t carry = 0;
	size_t size = 0;

	size = MAX(rr(a), rr(b)) + MAX(rl(a), rl(b)) - 1;

	for (;i < a->len || j < b->len; size--, c->len++) {
		sum = _pl(a, b, &i, c->len) + _pl(b, a, &j, c->len) + carry;
		carry = 0;
		if(sum >= base) {
			carry = 1;
			sum -= base;
		}
		c->number[size] = sum;
	}
	/* move the entire number to the right 1 place in the case that
	   addition has a left over carry, array offsets also work */
	if (carry) {
		for(i = c->len+1;i > 0; i--)
			c->number[i] = c->number[i-1];
		c->number[0] = 1;
		c->len++;
		c->lp++;
	}
	return c;
}

/* the actual subtraction */
fxdpnt *arb_sub_inter(const fxdpnt *a, const fxdpnt *b, fxdpnt *c, int base)
{
	size_t i = 0;
	size_t j = 0;
	ARBT sum = 0;
	int8_t borrow = 0;
	int8_t mborrow = -1; /* mirror borrow must be -1 */
	ARBT mir = 0;
	UARBT *array = NULL;
	UARBT *tmp = NULL;
	ARBT hold = 0;
	size_t size = 0;
	size_t array_allocated = (MAX(rr(a), rr(b)) + MAX(rl(a), rl(b)) + 1);

	array = arb_malloc(array_allocated * sizeof(UARBT));
	size = MAX(rr(a), rr(b)) + MAX(rl(a), rl(b)) - 1;

	for (;i < a->len || j < b->len; size--, c->len++) {
		hold = _pl(a, b, &i, c->len) - _pl(b, a, &j, c->len);
		sum = hold + borrow;
		mir = hold + mborrow;
		borrow = mborrow = 0;
		if(sum < 0) {
			borrow = -1;
			sum += base;
		}
		if(mir < 0) {
			mborrow = -1;
			mir += base;
		}
		c->number[size] = sum;
		array[size] = (base-1) - mir;
	}
	/* a left over borrow indicates that the zero threshold was crossed */
	if (borrow == -1) {
		tmp = c->number;
		c->number = array;
		c->allocated = array_allocated; // TODO: this should be scaled
		free(tmp);
		arb_flipsign(c);
	}else {
		free(array);
	}
	return c;
}

/* wrappers and identity redirection for add and sub */
fxdpnt *arb_add(const fxdpnt *a, const fxdpnt *b, fxdpnt *c, int base)
{
	c = arb_add2(a, b, c, base);
	c = remove_leading_zeros(c);
	return c;
}

fxdpnt *arb_sub(const fxdpnt *a, const fxdpnt *b, fxdpnt *c, int base)
{
	c = arb_sub2(a, b, c, base);
	c = remove_leading_zeros(c);
	return c;
}

void sub(const fxdpnt *a, const fxdpnt *b, fxdpnt **c, int base, char *m)
{ 
	_internal_debug; 
	*c = arb_sub(a, b, *c, base);
	_internal_debug_end;
}

void add(const fxdpnt *a, const fxdpnt *b, fxdpnt **c, int base, char *m)
{
	_internal_debug; 
	*c = arb_add(a, b, *c, base);
	_internal_debug_end;
} 

void decr(fxdpnt **c, int base, char *m)
{
	_internal_debug; 
	*c = arb_sub(*c, one, *c, base);
	_internal_debug_end;
}

void incr(fxdpnt **c, int base, char *m)
{
	_internal_debug; 
	*c = arb_add(*c, one, *c, base);
	_internal_debug_end;
}

fxdpnt *arb_add2(const fxdpnt *a, const fxdpnt *b, fxdpnt *c, int base)
{
	fxdpnt *c2 = arb_expand(NULL, MAX(rr(a), rr(b)) + MAX(rl(a), rl(b)) + 1);
	c2->lp = MAX(rl(a), rl(b));
	arb_init(c2);
	if (a->sign == '-' && b->sign == '-') {
		arb_flipsign(c2);
		//c2 = arb_add_inter(a, b, c2, base);
		c2 = newadd(a, b, c2, base);
	}
	else if (a->sign == '-')
		c2 = arb_sub_inter(b, a, c2, base);
	else if (b->sign == '-')
		c2 = arb_sub_inter(a, b, c2, base);
	else
		//c2 = arb_add_inter(a, b, c2, base);
		c2 = newadd(a, b, c2, base);
	arb_free(c);
	return c2;
}

fxdpnt *arb_sub2(const fxdpnt *a, const fxdpnt *b, fxdpnt *c, int base)
{
	fxdpnt *c2 = arb_expand(NULL, MAX(rr(a), rr(b)) + MAX(rl(a), rl(b)) + 1);
	c2->lp = MAX(rl(a), rl(b));
	arb_init(c2);
	if (a->sign == '-' && b->sign == '-')
	{
		arb_flipsign(c2);
		c2 = arb_sub_inter(a, b, c2, base);
	}
	else if (a->sign == '-'){
		arb_flipsign(c2);
		//c2 = arb_add_inter(a, b, c2, base);
		c2 = newadd(a, b, c2, base);
	}
	else if (b->sign == '-' || a->sign == '-')
		//c2 = arb_add_inter(a, b, c2, base);
		c2 = newadd(a, b, c2, base);
	else
		c2 = arb_sub_inter(a, b, c2, base);
	arb_free(c);
	return c2;
}

void sub2(const fxdpnt *a, const fxdpnt *b, fxdpnt **c, int base, char *m)
{ 
	_internal_debug; 
	*c = arb_sub2(a, b, *c, base);
	_internal_debug_end;
}

void add2(const fxdpnt *a, const fxdpnt *b, fxdpnt **c, int base, char *m)
{
	_internal_debug; 
	*c = arb_add2(a, b, *c, base);
	_internal_debug_end;
}

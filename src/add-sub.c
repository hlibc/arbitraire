#include "internal.h"

/* Copyright 2017-2019 CM Graff */


/*
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

	Addition:

		six_loop_add:
			
			This addition function aims to be fully optimized
			in regards to the number of conditional decisions
			per loop.

		arb_add_inter:

			arb_add_inter makes use of the _pl function in
			order to express addition as consisely as possible.
			This function has the drawback of fitting many
			conditionals into the same looping code block --
			some of which may or may not be optimized by the
			compiler
	Subtraction:

		arb_sub_inter:

			See arb_add_inter for details about arb_sub_inter
			as they are nearly identical functions.

		five_loop_sub:

			Work in progress 

			Correct output

			Note that most implementations are using compare()
		       	beforehand for subtractions

*/

fxdpnt *five_loop_sub(const fxdpnt *a, const fxdpnt *b, fxdpnt *c, int base)
{
	size_t i = 0;
	size_t k = 0;	
	size_t j = 0;
	size_t len = 0;
	ARBT sum = 0;
	int8_t borrow = 0;
	int8_t mborrow = -1; /* mirror borrow must be -1 */
	ARBT mir = 0;
	UARBT *array = NULL;
	UARBT *tmp = NULL;
	ARBT hold = 0;
	
	size_t array_allocated = (MAX(rr(a), rr(b)) + MAX(rl(a), rl(b)) + 1);

	array = arb_malloc(array_allocated * sizeof(UARBT));
	j = MAX(rr(a), rr(b)) + MAX(rl(a), rl(b)) -1;

	size_t y = b->len-1;
	size_t z = a->len-1;

	/* take care of differing tails to the right of the radix */
	if (rr(a) > rr(b)) {
		len = rr(a) - rr(b);
		for (i=0;i < len; i++, j--, z--, c->len++) {
			hold = a->number[z];
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
			c->number[j] = sum;
			array[j] = (base-1) - mir;
		}
	}
	/* perform subtraction from 0 on the bottom long tail */
	else if (rr(b) > rr(a)) {
		len = rr(b) - rr(a);
		for (k=0;k < len; k++, j--, y--, c->len++) {
			hold = 0 - b->number[y];
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
			c->number[j] = sum;
			array[j] = (base-1) - mir;
		}
	}

	for (;i < a->len && k < b->len; j--, c->len++, i++, k++, z--, y--) { 
		hold = a->number[z] - b->number[y]; 
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
		c->number[j] = sum;
		array[j] = (base-1) - mir;
	}

	for (;i < a->len; j--, c->len++, i++, z--) { 
		hold = a->number[z]; 
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
		c->number[j] = sum;
		array[j] = (base-1) - mir;
	}

	for (;k < b->len; j--, c->len++, k++, y--) { 
		hold = 0 - b->number[y];
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
		c->number[j] = sum;
		array[j] = (base-1) - mir;
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

fxdpnt *six_loop_add(const fxdpnt *a, const fxdpnt *b, fxdpnt *c, int base)
{
	/* This addition function is designed to make a small
	 * number of conditional decisions per loop. Hence, why
	 * there are so many loops. Using temporary variables
	 * doesn't save much space over using additional loops
	 * (maybe 6 loc?) so we go ahead and use the extra loops.
	*/

	size_t i = 0;
	size_t k = 0;
	size_t j = MAX(rr(a), rr(b)) + MAX(rl(a), rl(b)) -1;
	size_t len = 0;
	int sum = 0;
	int carry = 0;
	size_t z = a->len -1;
	size_t y = b->len -1;

	/* take care of differing tails to the right of the radix */
	if (rr(a) > rr(b)) {
		len = rr(a) - rr(b);
		for (i=0;i < len; i++, j--, z--, c->len++) {
			c->number[j] = a->number[z];
		}
	}
	else if (rr(b) > rr(a)) {
		len = rr(b) - rr(a);
		for (k=0;k < len; k++, j--, y--, c->len++) {
			c->number[j] = b->number[y];
		}
	}

	/* numbers are now compatible for a straight-forward add */
	for (;i < a->len && k < b->len; i++, j--, k++, z--, y--, c->len++) {
		sum = a->number[z] + b->number[y] + carry; 
		carry = 0;
		if (sum >= base) {
			sum -= base;
			carry = 1;
		}
		c->number[j] = sum;
	}

	/* one number may be longer than the other to the left */
	for (;i < a->len; i++, j--, z--, c->len++) { 
		sum = a->number[z] + carry; 
		carry = 0;
		if (sum >= base) {
			sum -= base;
			carry = 1;
		}
		c->number[j] = sum;
	}

	for (;k < b->len; j--, k++, y--, c->len++) { 
		sum = b->number[y] + carry; 
		carry = 0;
		if (sum >= base) {
			sum -= base;
			carry = 1;
		}
		c->number[j] = sum;
	}

	/* handle the final left over carry */
	if (carry) {
		for(i = c->len+1;i > 0; i--) {
			c->number[i] = c->number[i-1];
		}
		c->number[0] = 1;
		c->len++;
		c->lp++;
	}

	return c;
}

/* _pl() draws from an imaginary array of zeros and allows add and sub to work
 * on numbers with varying magnitudes in a concise manner
*/
static UARBT _pl(const fxdpnt *a, const fxdpnt *b, size_t *cnt, size_t r)
{
	UARBT temp = 0;
	if (rr(a) < rr(b))
		if(rr(b) - rr(a) > r)
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
	 * addition has a left over carry, array offsets also work
	 */
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
/* identity redirection for add and sub */
fxdpnt *arb_add2(const fxdpnt *a, const fxdpnt *b, fxdpnt *c, int base)
{
	fxdpnt *c2 = arb_expand(NULL, MAX(rr(a), rr(b)) + MAX(rl(a), rl(b)) + 1);
	arb_init(c2);
	c2->lp = MAX(rl(a), rl(b));
	if (a->sign == '-' && b->sign == '-') {
		arb_flipsign(c2);
		c2 = six_loop_add(a, b, c2, base);
	}
	else if (a->sign == '-') {
		c2 = arb_sub_inter(b, a, c2, base);
		//c2 = five_loop_sub(b, a, c2, base);
	}
	else if (b->sign == '-') {
		c2 = arb_sub_inter(a, b, c2, base);
		//c2 = five_loop_sub(a, b, c2, base);
	}
	else {
		c2 = six_loop_add(a, b, c2, base);
	}
	arb_free(c);
	return c2;
}

fxdpnt *arb_sub2(const fxdpnt *a, const fxdpnt *b, fxdpnt *c, int base)
{
	fxdpnt *c2 = arb_expand(NULL, MAX(rr(a), rr(b)) + MAX(rl(a), rl(b)) + 1);
	arb_init(c2);
	c2->lp = MAX(rl(a), rl(b));
	if (a->sign == '-' && b->sign == '-')
	{
		arb_flipsign(c2);
		c2 = arb_sub_inter(a, b, c2, base);
		//c2 = five_loop_sub(a, b, c2, base);
	}
	else if (a->sign == '-'){
		arb_flipsign(c2);
		c2 = six_loop_add(a, b, c2, base);
	}
	else if (b->sign == '-' || a->sign == '-') {
		c2 = six_loop_add(a, b, c2, base);
	}
	else {
		c2 = arb_sub_inter(a, b, c2, base);
		//c2 = five_loop_sub(a, b, c2, base);
	}
	arb_free(c);
	return c2;
}

/* wrappers for add and sub */
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

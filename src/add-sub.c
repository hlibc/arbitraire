#include <arbitraire/arbitraire.h>

void arb_reverse(fxdpnt *x)
{
	size_t i = 0, half = x->len / 2;
	ARBT swap = 0;
	for (;i < half; i++){
		swap = x->number[i];
		x->number[i] = x->number[x->len - i - 1];
		x->number[x->len - i - 1] = swap;
	}
}

ARBT arb_place(fxdpnt *a, fxdpnt *b, size_t *cnt, size_t r)
{
	ARBT temp = 0;
	if ((rr(a)) < (rr(b)))
		if((rr(b)) - (rr(a)) > r)
			return 0;
	if (*cnt < a->len){
		temp = a->number[a->len - *cnt - 1];
		(*cnt)++;
		return temp;
	}
	(*cnt)++;
	return 0;
}

fxdpnt *arb_add_inter(fxdpnt *a, fxdpnt *b, fxdpnt *c, int base)
{
	size_t i = 0, j = 0, r = 0;
	int sum = 0, carry = 0;

	for (; i < a->len || j < b->len;c->len++, ++r){
		sum = arb_place(a, b, &i, r) + arb_place(b, a, &j, r) + carry;
		carry = 0;
		if(sum >= base){
			carry = 1;
			sum -= base;
		}
		c->number[c->len] = sum;
	}
	if (carry){
		c->number[c->len++] = 1;
		c->lp += 1;
	}
	arb_reverse(c);

	return c;
}


fxdpnt *arb_sub_inter(fxdpnt *a, fxdpnt *b, fxdpnt *c, int base)
{
	size_t i = 0, j = 0, r = 0;
	int sum = 0, borrow = 0;
	int mborrow = -1; // mirror borrow must be -1
	int mir = 0;
	size_t z = 0, y = 0; // dummy variables for the mirror
	ARBT *array;

	array = arb_malloc((MAX(a->len, b->len) * 2) * sizeof(ARBT));

	for (; i < a->len || j < b->len;c->len++, ++r){
		mir = arb_place(a, b, &y, r) - arb_place(b, a, &z, r) + mborrow; // mirror
		sum = arb_place(a, b, &i, r) - arb_place(b, a, &j, r) + borrow;

		borrow = 0;
		if(sum < 0){
			borrow = -1;
			sum += base;
		}
		c->number[c->len] = sum;
		// maintain a mirror for subtractions that cross the zero threshold
		y = i;
		z = j;
		mborrow = 0;
		if(mir < 0){
			mborrow = -1;
			mir += base;
		}
		array[c->len] = (base-1) - mir;
	}
	// a left over borrow indicates that the zero threshold was crossed
	if (borrow == -1){
		free(c->number);
		c->number = array;
		arb_flipsign(c);
	}else 
		free(array);
	arb_reverse(c);
	
	return c;
}

fxdpnt *arb_add(fxdpnt *a, fxdpnt *b, fxdpnt *c, int base)
{
	fxdpnt *c2 = arb_expand(NULL, (a->len + b->len) * 2);
	c2->lp = MAX(a->lp, b->lp);
	arb_init(c2);
	if (a->sign == '-' && b->sign == '-') {
		arb_flipsign(c2);
		c2 = arb_add_inter(a, b, c2, base);
	}
	else if (a->sign == '-')
		c2 = arb_sub_inter(b, a, c2, base);
	else if (b->sign == '-')
		c2 = arb_sub_inter(a, b, c2, base);
	else
		c2 = arb_add_inter(a, b, c2, base);
	if (c)
		arb_free(c);
	c2 = remove_leading_zeros(c2);
	return c2;
}

fxdpnt *arb_sub(fxdpnt *a, fxdpnt *b, fxdpnt *c, int base)
{
	fxdpnt *c2 = arb_expand(NULL, (a->len + b->len) * 2);
	c2->lp = MAX(a->lp, b->lp);
	arb_init(c2);
	if (a->sign == '-' && b->sign == '-')
	{
		arb_flipsign(c2);
		c2 = arb_sub_inter(a, b, c2, base);
	}
	else if (a->sign == '-'){
		arb_flipsign(c2);
		c2 = arb_add_inter(a, b, c2, base);
	}
	else if (b->sign == '-' || a->sign == '-')
		c2 = arb_add_inter(a, b, c2, base);
	else
		c2 = arb_sub_inter(a, b, c2, base);
	if (c)
		arb_free(c);
	c2 = remove_leading_zeros(c2);
	return c2;
}


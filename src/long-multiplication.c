#include "internal.h"

/* Copyright 2017-2019 CM Graff */

/*
	arb_mul_core:

	This is a partial carry variation on the traditional "school book" long
	multiplication algorithm. Arrays are accessed via n-1 to aid in using
	unsigned types.

	A somewhat novel method for making the partial carry long 
	multiplication algorithm self-zeroing is used. In arbitraire this
	allows arb_mul_core to work with the implementation of Knuth's TAOCP 
	vol 2 algorithm D (division (src/div.c)) and allows arb_mul_core to 
	stand alone.

	An optimization is provided which tracks trailing zeros from the
	operands and moves them onto the answer. It hypothetically increases
	the speed of the multiplication when trailing zeros are present. Which
	may ultimately help karatsuba multiplication a great deal.

	The return value of arb_mul_core represents the magnitude of the
	product.

	arb_mul:

	arb_mul() is the actual interface that is intended to be used.

	arb_mul2() is a wrapper for arb_mul_core which provides memory
	allocation but does not strip zeros like arb_mul

	arb_mul_comba_core is a function that operates on the fundamental datum
	using comba multiplication. it is memory heavy but may produce a speedup
	by moving the carrying to a final step.
*/

static void _pr(const UARBT *a, size_t len)
{
	size_t i = 0;
	for (i=0;i<len;++i)
		fputc(arb_highbase(a[i]), stdout);
	fputc('\n', stdout);
}
size_t arb_mul_comba_core(const UARBT *a, size_t alen, const UARBT *b, size_t blen, UARBT *c, int base)
{
	/* Comba multiplication. This algorithm is still under construction */

	/* TODO: perform some tests on the unsigned k-1 incrementor convention and make sure that
		it is not creating extra leading zeros that must be stripped
		-- this applies to normal long multiplication too!  

		operand arrangement affects the number of rows -- explore arranging operands
		to conserve memory.

		Comba can have the carries done per row (comba), or have all of the rows 
		summated and then the carries performed (fast comba).

		Fast comba can only be used for smaller inputs or it will lead to
		type overflow. This would be referred to as the acyclic or linear 
		convolution 

		Ideally, comba rows would stary at index 1 so that the carries can
		be performed without reallocation or copying memory. This would have
		the expense of a leading zero, but in this case, that leading zero
		may be naturally discarded for the rows and probably will only result
		in a simgle leading zero for the total/
	 */

	
	UARBT prod = 0;
	UARBT carry = 0;
	UARBT sum = 0;
	size_t i = 0;
	size_t j = 0;
	size_t k = 0;
	size_t ret = 0;

	/* number of rows and the length of rows are the same -- plus extra zeros */
	size_t numrows = MAX(alen, blen);
	size_t rowlen = alen + blen + 1;

	UARBT **rows = arb_malloc((numrows * 10) * sizeof(UARBT));
	memset(c, 0, alen +blen);
	
	size_t rowc = 0; 
	size_t last = 0;
	for (i = alen; i > 0 ; i--) {
		rows[rowc] = arb_calloc(1,( rowlen * 100) * sizeof(UARBT) ); 
		for (j = blen, k = i + j; j > 0 ; j--, k--){ 
			rows[rowc][k-1] = a[i-1] * b[j-1];
		} 
		++rowc; 
	}


	size_t z = 0;
	size_t endlen = numrows;
	/* pass over all rows */
	for(;z<rowc;++z) {
		
		/* pass over all digits in a row */
		for (j=rowlen;j>0; j--){
			carry = rows[z][j] / base;
			rows[z][j] = rows[z][j] % base;
			rows[z][j - 1] += carry;
			
			//printf("\n");
	                //for(i = 0;i<rowlen;++i) {
        	        //        printf("%u ", rows[z][i]);
	                //}
	                //printf("\n");
		} 
		if ( j == 0 && carry)
		{

			for (i = rowlen + 1; i > 0; i--) {
				rows[z][i] = rows[z][i-1];
			}
			rows[z][0] = 1;
			//endlen += 1;
		}
		/* summate the current row into the total 'c' */
		for (carry=0, i=rowlen; i>0;--i,--j) {
			sum = c[i] + rows[z][i] + carry;
			carry = 0;
			if(sum >= base) {
				carry = 1;
				sum -= base;
			}
			c[i] = sum;
		}
		if (carry) {
			for (i = endlen + 1; i > 0; i--) {
				c[i] = c[i-1];
			}
			c[0] = 1;
			endlen += 1;
		}
		//printf("\ntotal\n");
                /* now print the row  and see if we got it right */
                //for(i = 0;i<endlen;++i) {
                  //      printf("%u ", c[i]);
                //}
                /* now sum the row into a running total */
                //printf("\n");

	}
	return ret;
}

size_t arb_mul_core(const UARBT *a, size_t alen, const UARBT *b, size_t blen, UARBT *c, int base)
{
	/* TODO: a memset is probably faster than the embedded conditional
	 * so the assembly code needs to be examined and some timing tests performed
	 */
	UARBT prod = 0;
	UARBT carry = 0;
	size_t i = 0;
	size_t j = 0;
	size_t k = 0;
	size_t last = 0;
	size_t ret = 0;

	c[0] = 0;
	c[alen+blen-1] = 0;

	/* move zeros onto the solution and reduce the mag of the operands */
	for (;alen > 3 && ! a[alen-1]; ++ret) {
		c[--alen + blen -1] = 0;
	}
	for (;blen > 3 && ! b[blen-1]; ++ret) {
		c[alen + --blen -1] = 0;
	}

	/* outer loop -- first operand */
	for (i = alen; i > 0 ; i--){
		last = k;
		/* inner loop, second operand */
		for (j = blen, k = i + j, carry = 0; j > 0 ; j--, k--){
			prod = a[i-1] * b[j-1] + c[k-1] + carry;
			carry = prod / base;
			c[k-1] = prod % base;
		}
		/* self zeroing */
		if (k != last) { 
			++ret;
			c[k-1] = 0;
		}
		c[k-1] += carry;
	}

	return ret;
}

fxdpnt *arb_mul_by_one(const fxdpnt *a, const fxdpnt *b, fxdpnt *c)
{
	if (arb_compare(a, one) == 0) {
		fxdpnt *c2 = arb_expand(NULL, a->len);
		c2 = arb_copy(c2, b);
	        arb_free(c);
		c2 = remove_leading_zeros(c2);
	        return c2;

	} else if (arb_compare(b, one) == 0) {
		fxdpnt *c2 = arb_expand(NULL, b->len);
		c2 = arb_copy(c2, a);
	        arb_free(c);
		c2 = remove_leading_zeros(c2);
	        return c2;
	}
	else
		return NULL;
}


fxdpnt *arb_mul(const fxdpnt *a, const fxdpnt *b, fxdpnt *c, int base, size_t scale)
{ 
	
	//if ((c = arb_mul_by_one(a, b, c)) != NULL)
	//	return c;
	/* use karatsuba multiplication if either operand is over 1000 digits */
	if (MAX(a->len, b->len) > 1000)
		return arb_karatsuba_mul(a, b, c, base, scale);

	c = arb_mul2(a, b, c, base, scale);
	
	c = remove_leading_zeros(c);
	return c;
}


fxdpnt *arb_comba(const fxdpnt *a, const fxdpnt *b, fxdpnt *c, int base, size_t scale)
{
	fxdpnt *c2 = arb_expand(NULL,( a->len + b->len) * 10);
	arb_setsign(a, b, c2);
        arb_mul_comba_core(a->number, a->len, b->number, b->len, c2->number, base);
        c2->lp = rl(a) + rl(b);
        c2->len = MIN(rr(a) + rr(b), MAX(scale, MAX(rr(a), rr(b)))) + c2->lp;
        arb_free(c);
	c2 = remove_leading_zeros(c2);
        return c2;
}
fxdpnt *arb_mul2(const fxdpnt *a, const fxdpnt *b, fxdpnt *c, int base, size_t scale)
{
	fxdpnt *c2 = arb_expand(NULL, a->len + b->len);
	arb_setsign(a, b, c2);
        arb_mul_core(a->number, a->len, b->number, b->len, c2->number, base);
        c2->lp = rl(a) + rl(b);
        c2->len = MIN(rr(a) + rr(b), MAX(scale, MAX(rr(a), rr(b)))) + c2->lp;
        arb_free(c);
	
        return c2;
}

void mul(const fxdpnt *a, const fxdpnt *b, fxdpnt **c, int base, size_t scale, char *m)
{
	_internal_debug;
	*c = arb_mul(a, b, *c, base, scale);
	_internal_debug_end;
}

void mul2(const fxdpnt *a, const fxdpnt *b, fxdpnt **c, int base, size_t scale, char *m)
{
	_internal_debug;
	*c = arb_mul2(a, b, *c, base, scale);
	_internal_debug_end;
}

void debugmul(const fxdpnt *a, const fxdpnt *b, fxdpnt **c, int base, size_t scale, char *m)
{
	_internal_debug;
	*c = arb_mul(a, b, *c, base, scale);
	_internal_debug_end;
}



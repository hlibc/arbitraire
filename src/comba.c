#include "internal.h"

/* Copyright 2017-2019 CM Graff */

/* 
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

		Ideally, comba rows would start` at index 1 so that the carries can
		be performed without reallocation or copying memory. This would have
		the expense of a leading zero, but in this case, that leading zero
		may be naturally discarded for the rows and probably will only result
		in a simgle leading zero for the total
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


#include "internal.h"

/* Copyright 2017-2019 CM Graff */


/*
	This is an implementation of Donald Knuth's TAOCP vol 2 Algorithm D 

	Knuth's TAOCP vol 2 Algorithm D is based on long division however it is
	quite a bit different than the typical school-book algorithm method.
		* A normalization step is provided
			This step is of the form such that a new numerator
			and denominator are derived via:
				n = base / D[0] + 1
				N * n = (new_numerator)
				D * n = (new_denominator)
				N/D = new_numerator/new_denominator
			The effect of dividing the new numerator and
			denominator is identical to that of the original

		* 2 guesses are made early in the guessing stategem
		* Finally, the answer is derived from a long multiplication
		  followed by a long subtraction and a long addition.
		  
	Care was taken to reproduce Knuth's original algorithm which was
	written in MIX theoretical assembly. Wherever possible, I tried to
	use Knuth's variable naming conventions and duplicate his usage of
	goto statements in order to produce an educationally oriented
	interpretation that emphasizes his methods.

	I have omitted the final step of calculating the modulus operation
	for the sake of efficiency, though it should probably be added in
	at some future point. This would likely produce a speedup over
	arbitraire's current method which implements modulus in terms of

		modulus(a) = a - (b * (a / b))
	
	see src/modulo.c for this operation.

	This algorithm deviates from Knuth's method by fusing the normalization 
	and temporary variable copy downs. This does not optimize the case of a 
	normalization factor of (1) but instead trades it off for a smaller code
       	footprint.

	TODO: don't exit on zero
	TODO: strip trailing zeros from the denominator


	History:
	The modern method of long division is attributed to Henry Briggs circa 
	1600
*/

int _long_sum(UARBT *u, size_t i, const UARBT *v, size_t k, int b, uint8_t lever)
{
	uint8_t carrborr = 0;
	ARBT val = 0;
	for (;k+1 > 0; i--, k--) {
		/* addition */
		if (lever == 0) {
			val = u[i] + v[k] + carrborr;
			carrborr = 0;
			if (val >= b) {
				val -= b;
				carrborr = 1;
			}
		/* subtraction */
		}else {
			val = u[i] - v[k] - carrborr; 
			carrborr = 0;
			if (val < 0) {
				val += b;
				carrborr = 1;
			}
		}
		u[i] = val;
	}
	return carrborr;
}

fxdpnt *arb_div_inter(const fxdpnt *num, const fxdpnt *den, fxdpnt *q, int b, size_t scale)
{
	UARBT *u = NULL;
	UARBT *v = NULL;
	UARBT *vf = NULL;
	UARBT *temp = NULL;
	UARBT *p = NULL;
	UARBT qg = 0;
	UARBT norm = 0;
	size_t lea = 0;
	size_t leb = 0;
	size_t i = 0;
	size_t j = 0;

	if (iszero(den) == 0) {
		fputs("Divide by zero\n", stderr);
		return NULL;
	}

	/* temporary storage and storage for normalized den and num */
	u = arb_calloc(1, (num->len + rr(den) + 3 + scale) * sizeof(UARBT));
	vf = v = arb_calloc(1, (den->len + rr(num) + 3 + scale) * sizeof(UARBT));
	temp = arb_malloc((den->len+1) * sizeof(UARBT)); 
	p = den->number;
	leb = den->len;

	/* find the first real value for normalization (strip zeros) */
	for (;!*p; p++, leb--);

	/* normalization is fused with copy down to temporary storage */
	norm = (b / (p[0] + 1));
	arb_mul_core(num->number, num->len, &norm, 1, u, b);
	arb_mul_core(p, leb, &norm, 1, v, b);
	if (!*v) /* deal with a possible zero from arb_mul_core */
		v++;

	/* compute the scales for the final solution */
	lea = rl(num) + rr(den);
	q->lp = 1;
	if (leb > lea+scale) {
		q->len = q->lp + scale;	
		goto end;
	} else {
		if (!(leb>lea))
			q->lp = lea - leb + 1;
	}
	q->len = q->lp + scale;

	/* begin the division operation */
	if (leb > lea)
		j = (leb-lea);

	for (qg = b-1;i <= lea+scale-leb; ++i, ++j, qg = b-1) {
		if (v[0] != u[i])
			qg = (u[i]*b + u[i+1]) / v[0];
		/* early guesses */
		if (v[1] * qg > (u[i] * b + u[i+1] - v[0] * qg) * b + u[i+2]) {
			qg = qg - 1;
			if (v[1] * qg > (u[i] * b + u[i+1] - v[0] * qg) * b + u[i+2])
				qg = qg - 1;
		} 
		/* D4. [Multiply and Subtract] */
		if (qg != 0) {
			arb_mul_core(v, leb, &qg, 1, temp, b);
			if (!(_long_sum(u+leb, i, temp, leb, b, 1)))
				goto D7;
			qg = qg - 1;
			if (_long_sum(u+leb, i, v, leb-1, b, 0))
				u[0] = 0;
		}
		D7: /* D7 */
		q->number[j] = qg;
	}
	end:
	q = remove_leading_zeros(q);
	free(temp);
	free(u);
	free(vf);
	return q;
}

fxdpnt *arb_div(const fxdpnt *a, const fxdpnt *b, fxdpnt *c, int base, size_t scale)
{
	fxdpnt *c2 = arb_expand(NULL, a->len + b->len + scale);
	arb_init(c2);
	arb_setsign(a, b, c2);
	c2 = arb_div_inter(a, b, c2, base, scale);
	arb_free(c);
	return c2;
}

void divv(const fxdpnt *num, const fxdpnt *den, fxdpnt **c, int b, size_t scale, char *m)
{
	_internal_debug;
	*c = arb_div(num, den, *c, b, scale);
	_internal_debug_end;
}


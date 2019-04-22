#include "internal.h"
/*
	Copyright Bao Hexing 2018
*/

static int is_trailing_zeros(const fxdpnt *a, size_t idx) {
	while (idx < a->len) {
		if (a->number[idx] != 0) return 0;
		++idx;
	}
	return 1;
}

int arb_compare(const fxdpnt *a, const fxdpnt *b) {
	size_t a_pos = 0;
	size_t b_pos = 0;
	int result = 0;

	if (a->sign == '-' && b->sign == '+') return -1;
	if (a->sign == '+' && b->sign == '-') return 1;

	/* This may be better implemented as a raw for-loop to avoid any extra
	 * variable and function overhead in count_leading_zeros()
	 */
	a_pos = count_leading_zeros(a);
	b_pos = count_leading_zeros(b);

	if (a->lp - a_pos > b->lp - b_pos)
		result = 1;
	else if (a->lp - a_pos < b->lp - b_pos)
		result = -1;
	else {
		for (; a_pos < a->len && b_pos < b->len &&
			   a->number[a_pos] == b->number[b_pos];
			 a_pos++, b_pos++)
			;

		if (a_pos == a->len && b_pos < b->len && !is_trailing_zeros(b, b_pos))
			result = -1;
		else if (b_pos == b->len && b_pos < a->len &&
				 !is_trailing_zeros(a, a_pos))
			result = 1;
		else if (a_pos == a->len && b_pos == b->len)
			result = 0;
		else
			result = a->number[a_pos] - b->number[b_pos];
	}

	if (a->sign == '-') {  // and also b->sign == '-'
		result = 0 - result;
	}

	return result;
}

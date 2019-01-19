#include "internal.h"

fxdpnt *old_div(fxdpnt *num, fxdpnt *den, fxdpnt *q, int b, size_t scale)
{
	scale = scale;
	size_t i = 0;
	size_t j = 0;
	size_t z = 0;
	size_t width = num->len + den->len;
	UARBT *u = arb_malloc(width);
	UARBT *v = arb_malloc(width);
	int sum = 0;
	int rec = 0;
	q->lp = 0;
	q->len = 0;
	
	arb_init(q);
	
	_arb_memset(u, 0, width);
	_arb_copy_core(u, num->number, num->len);
	arb_expand(q, num->len + scale);
	_arb_memset(q->number, 0, num->len + scale);
	q->number[z] = 0;

	/* compute the scales for the final solution */
        size_t lea = rl(num) + rr(den);
	size_t leb = den->len;
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


	for (; z < num->len;)
	{ 
		for (rec = 0, i = 0, j = z ; i < den->len; j++ ,i++)
		{ 
			sum = u[j] - den->number[i];
			if (sum < 0)
			{
				if (j == z)
				{ 
					u[j + 1] += (u[j] * b);
					z++;
					q->len++;
					q->number[z] = 0;
				}
			 	else
				{
					u[j - 1] -= 1;
					u[j] += b;
				}
				rec = 1;
				break;
			}
			v[j] = sum;
		}
		if (rec == 0)
		{
			_arb_copy_core(u, v, j);
			q->number[z] += 1;
		}
	} 
	end:
	free(u);
	free(v);
	return q;
}





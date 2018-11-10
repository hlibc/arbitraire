#include <arbitraire/arbitraire.h>

fxdpnt *old_div(fxdpnt *num, fxdpnt *den, fxdpnt *q, int b, size_t scale)
{
	size_t i = 0;
	size_t j = 0;
	size_t z = 0;
	size_t width = num->len + den->len;
	char *mir = arb_malloc(width);
	char *tmir = arb_malloc(width);
	int sum = 0;
	int rec = 0;
	q->lp = 0;
	q->len = 0;
	
	arb_init(q);
	
	memset(mir, 0, width);
	memcpy(mir, num->number, num->len);
	memset(q->number, 0, num->len);
	q->number[z] = 0;

	for (; z < num->len;)
	{ 
		for (rec = 0, i = 0, j = z ; i < den->len ; j++ ,i++)
		{ 
			sum = (mir[j]) - (den->number[i]);
			if (sum < 0)
			{
				if (j == z)
				{ 
					mir[j + 1] += ((mir[j]) * b);
					z++;
					q->len++;
					q->number[z] = 0;
				}
			 	else
				{
					mir[j - 1] -= 1;
					mir[j] += b;
				}
				rec = 1;
				break;
			}
			tmir[j] = sum;
		}
		if (rec == 0)
		{
			memcpy(mir, tmir, j);
			q->number[z] += 1;
		}
	} 
	free(mir);
	free(tmir);
	return q;
}



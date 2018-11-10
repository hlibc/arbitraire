#include <arbitraire/arbitraire.h>

fxdpnt *old_div(fxdpnt *a, fxdpnt *b, fxdpnt *c, int base)
{
	size_t i = 0;
	size_t j = 0;
	size_t z = 0;
	size_t width = a->len + b->len;
	size_t diff = 0;
	size_t off = 0;
	char *mir = arb_malloc(width);
	char *tmir = arb_malloc(width);
	int sum = 0;
	int rec = 0;
	c->lp = 0;
	c->len = 0;
	
	arb_init(c);
	
	memset(mir, 0, width);
	memcpy(mir, a->number, a->len);
	memset(c->number, 0, a->len);
	c->number[z] = 0;

	for (; z < a->len;)
	{ 
		for (rec = 0, i = 0, j = z ; i < b->len ; j++ ,i++)
		{ 
			sum = (mir[j]) - (b->number[i]);
			if (sum < 0)
			{
				if (j == z)
				{ 
					mir[j + 1] += ((mir[j]) * base);
					z++;
					c->len++;
					c->number[z] = 0;
				}
			 	else
				{
					mir[j - 1] -= 1;
					mir[j] += base;
				}
				rec = 1;
				break;
			}
			tmir[j] = sum;
		}
		if (rec == 0)
		{
			memcpy(mir, tmir, j);
			c->number[z] += 1;
		}
	} 
	free(mir);
	free(tmir);
	return c;
}



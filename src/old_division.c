#include <arbitraire/arbitraire.h>

static size_t rl(fxdpnt *a)
{
	return a->lp;
}

fxdpnt *old_div(fxdpnt *a, fxdpnt *b, fxdpnt *c, int base)
{
	size_t i = 0;
	size_t j = 0;
	size_t z = 0;
	size_t width = a->len + b->len;
	size_t diff = 0;
	size_t off = 0;
	int *mir = arb_malloc(width);
	int *tmir = arb_malloc(width);
	int sum = 0;
	int rec = 0;
	
	

	arb_init(c);

	//if (arbprec_isnegati(a))
	//	arbprec_setsign(c);
	//if (arbprec_isnegati(b))
	//	arbprec_setsign(c);
	
	memset(mir + a->len, 0, width - a->len);
	memcpy(mir, a->number, a->len);
	c->number[z] = 0;

	/* count the zeros to the right of the radix before a non-zero value */


	while (b->number[off] == 0 && off < b->len)
		++off;
	/*	
	if (rl(a) < rl(b))
	{
		diff = rl(b) - rl(a) - 1;
		memset(c->number, 0, diff);
		c->len = z = diff;
		//c->number[z] = c->float_pos = 0 + off;
		c->number[z] = c->lp = 0 + off;
	}
	else if (rl(a) + 1> rl(b))
		c->lp = rl(a) - rl(b) + off + 1;
	else
		c->lp = rl(a) + off;
	*/
	for ( ; z < a->len + diff; )
	{
		for (rec = 0, i = off, j = z - diff ; i < b->len ; j++ ,i++)
		{
			sum = (mir[j]) - (b->number[i]);
			if ( sum < 0 )
			{
				if ( j == z - diff)
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
		if ( rec == 0 )
		{
			memcpy(mir, tmir, j);
			c->number[z] += 1;
		}
	} 
	free(mir);
	free(tmir);
	//c = strip_zeros(c);
	return c;
}



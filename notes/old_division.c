#include <arbitraire/arbitraire.h>

bigflt *arbitraire_div(bigflt *a, bigflt *b, bigflt *c)
{
	size_t i = 0;
	size_t j = 0;
	size_t z = 0;
	size_t width = a->len + b->len;
	size_t diff = 0;
	size_t off = 0;
	int *mir = arbitraire_malloc(sizeof(int) * width);
	int *tmir = arbitraire_malloc(sizeof(int) * width);
	int sum = 0;
	int rec = 0;
	a = strip_zeros(a);
	b = strip_zeros(b);

	arbitraire_init(c);

        if (arbitraire_isnegati(a))
                arbitraire_setsign(c);
        if (arbitraire_isnegati(b))
                arbitraire_setsign(c);
	
	setarray(mir + a->len, 0, width - a->len);
	copyarray(mir, a->number, a->len);
	c->number[z] = 0;

	/* count the zeros to the right of the radix before a non-zero value */
	while (b->number[off] == 0 && off < b->len)
		++off;
		
	if (rl(a) < rl(b))
	{
		diff = rl(b) - rl(a) - 1;
		setarray(c->number, 0, diff);
		c->len = z = diff;
		c->number[z] = c->float_pos = 0 + off;
	}
	else if (rl(a) + 1> rl(b))
		c->float_pos = rl(a) - rl(b) + off + 1;
	else
		c->float_pos = rl(a) + off;

	for ( ; z < a->len + diff; )
	{
		for (rec = 0, i = off, j  = z - diff ; i < b->len ; j++ ,i++)
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
			copyarray(mir, tmir, j);
			c->number[z] += 1;
		}
	} 
	free(mir);
	free(tmir);
	c = strip_zeros(c);
	return c;
}



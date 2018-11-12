#include <stdio.h>
#include <stdlib.h>
/* factorization can be sped up by handling
           the number logically.
                xxx*xxx is [10000, bbbbbb] and sigma_{n<bbbbbb}(y+1) 
                where b is base-1 and y is 10000
                
                This method can be deployed with the following code:
                a = arb_expand(a, b->len / 2);
                a->number[0] = 1;
                a->lp = a->len;
*/

int main(int argc, char *argv[])
{
	long long num1 = strtoll(argv[1], NULL, 10);
	long long num = 2;

	while (1)
	{
		if ( num * num == num1 )
		{ 
			break;
		} else if (num * num > num1 ) {
			--num;
			break;
		}
		++num;
	}
	printf("final result was %d\n", num);
	
	return 0;
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int num1 = strtol(argv[1], NULL, 10);

	if (num1 / 9 > -1)
	{
		printf("divide by nine was greater than -1\n");
	}
	else printf ("nines were too big\n");

	int base = 10;

	int num = 2;

	while (1)
	{
		if ( num * num <= num1 )
		{
			printf("%d * %d was <= %d\n", num, num, num1);
			if ( num * num >= num1 -base -1)
			{
				printf("%d * %d was >= %d -base-1\n", num, num, num1);
				break;
			}
			else
				printf("but it wasn't >=  %d -base-1\n", num1);
		}
		++num;
		if (num == base)
			break;
	}
	printf("final result was %d\n", num);
	
	return 0;
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int num1 = strtol(argv[1], NULL, 10);
	int num = 2;

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

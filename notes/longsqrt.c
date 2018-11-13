#include <stdio.h>
#include <stdlib.h>
double factor(double num, double num1)
{
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
	return num;
}

int main(int argc, char *argv[])
{
	double input = strtod(argv[1], NULL);
	

	printf("final result was %f\n", input);
	
	return 0;
}

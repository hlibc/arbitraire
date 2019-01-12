#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main(int argc, char *argv[])
{
	// N/D = N * 1/D
	double D = strtod(argv[1], 0);
	double N = strtod(argv[2], 0);
	double x = 0;
	x = 1.0 - D;
	int sign = 1;
	int i = 2;
	while (i < 10)
	{
		//D = 1.0 - D + pow(D,2.0) - pow(D, 3.0) + pow(D, 4.0);
		x+= pow(D, i) * sign;
		sign = -sign;
		i++;
	}
	printf ("%f\n", N*x);
	return 0;
}

//g(D) = 1/b = 1/1+x = 1 -D + D^2 - D^3 + D^4 - ...

#include <stdio.h>
#include <stdlib.h>
#define EULER 2.718281828459045


double taylor_series(double x, int needsone, int hyperbol, int isexp)
{
	int i = 0;
	int j = 0;
	int sign = 1;
	double d = 0.0;
	double z = 0.0;
	double y = 0.0;
	int bottom = 0;

	if(isexp) {
		bottom = 1;
		y = 1.0 + x;
		i = 2;
	}
	
/*
	while ( x >= (2*PI))
		x -= (2*PI);

	while ( x < 0 )
		x += (2*PI);
*/


	for (; 1; i++)
	{
		if (isexp)
			d = x;
		else
			d = 1.0;

		if (needsone)
			j = (2*i) + 1;
		else if (!isexp)
			j = (2*i);
		else
			j = i;

		for (; j > bottom; j--) {
			d *= (x / j);
		}

		if (hyperbol)
			y += d;
		else
	 		y += d * sign;

		sign = -sign;

		if (y == z)
			break;

		z = y;
	}

	return y;
}

int main(int argc , char *argv[])
{
	if (argc < 3) {
		fprintf(stderr, "Usage: [float] 1|2|3|4|5 where 1, 2, 3, 4 and 5 are sin cos, sinh,\ncosh and exp\n");
		return 1;
	}
	double f = atof(argv[1]);
	int type = strtol(argv[2], 0, 0);
	switch (type) {
		case 1: // sin
			f = taylor_series(f, 1, 0, 0);
			break;
		case 2: // cos
			f = taylor_series(f, 0, 0, 0);
			break;
		case 3: // sinh
			f = taylor_series(f, 1, 1, 0);
			break;
		case 4: // cosh
			f = taylor_series(f, 0, 1, 0);
			break;
		case 5: // exp
			f = taylor_series(f, 0, 1, 1);
			break;
		
	}
	printf("final result = %f\n", f);
	return 0;
}


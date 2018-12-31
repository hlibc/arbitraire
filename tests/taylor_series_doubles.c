#include <stdio.h>
#include <stdlib.h>
double taylor_series(double x, int needsone, int hyperbol)
{
	int i;
	int j;
	int sign = 1;
	double d;
	double z = 0;
	double y = 0;
/*
	while ( x >= (2*PI))
		x -= (2*PI);

	while ( x < 0 )
		x += (2*PI);
*/


	for (i = 0; 1; i++)
	{
		d = 1.0;
		if (needsone)
			j = (2*i) + 1;
		else
			j = (2*i);
		for (; j > 0; j--)
		{
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
		fprintf(stderr, "Usage: [float] 1|2|3|4 where 1, 2, 3, and 4 are sin cos, sinh,\n and cosh\n");
	}
	double f = atof(argv[1]);
	int type = strtol(argv[2], 0, 0);
	switch (type) {
		case 1: // sin
			f = taylor_series(f, 1, 0);
			break;
		case 2: // cos
			f = taylor_series(f, 0, 0);
			break;
		case 3: // sinh
			f = taylor_series(f, 1, 1);
			break;
		case 4: // cosh
			f = taylor_series(f, 0, 1);
			break;
	}
	printf("final result = %f\n", f);
	return 0;
}

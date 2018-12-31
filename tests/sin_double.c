#include <stdio.h>
#include <stdlib.h>
double taylor_series_sin(double x)
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
		printf("stages of 'd'\n");
                for (j = (2*i) + 1 ; j > 0; j--)
                {
                        d *= (x / j);
			printf("d = %f\n", d);
			
                }
                y += d * sign;
		printf("y = %f\n", y);
                sign = -sign;
                if ( y == z )
                        break;
                z = y;
        }

        return y;
}

int main(int argc , char *argv[])
{
	double f = atof(argv[1]);
	f = taylor_series_sin(f);
	printf("final result = %f\n", f);

}

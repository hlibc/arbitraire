#include <stdio.h>
#include <stdlib.h>

#define EULER 2.718281828459045
double taylor_series_exp(double x)
{
        int i;
        int n = 0; 
        double y = 0;
        double d = 0;
        double z = 0;

        if (x == 1.0) {
                y = EULER;
        } else if (x < 0) {
                y = 1.0 / taylor_series_exp(-x);
        } else {
                n = 2;
                y = 1.0 + x;
                while ( 1 )
                {
                        d = x; 
                        for (i = 2; i <= n; i++)
                        {
                                d *= x / i;
                        } 
                        y += d;
                        n++; 
                        if ( y == z )
                                break;
                        z = y;
                }
        }
        return y;
}

int main(int argc , char *argv[])
{
        double f = atof(argv[1]);
        f = taylor_series_exp(f);
        printf("final result = %f\n", f);
	return 0;
}


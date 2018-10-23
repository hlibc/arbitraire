#include <stdio.h>
#include <math.h>
double newton_iteration(double x, double eps)
{
        double guess = eps;
        double new_guess = 0;
        size_t i = 0;
        while (i < 100)
        {
                // method 1
                new_guess = guess * (2 - x * guess);
                // method 2
                //new_guess = guess + guess * ( 1 - x * guess); 
                if (guess == new_guess)
                        break;
                guess = new_guess;
                printf("%19.19lf\n", new_guess);
                ++i;

        }
        printf("iterations = %zu\n", i);
        return guess;
}

int main()
{
	double x = 123.123;
	double y = 10;

	printf("answer %f\n", x * newton_iteration(y, .0005));
}

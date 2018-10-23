#include <arbitraire/arbitraire.h>
#include <stdlib.h>
#include <time.h>
#define MAXIMA 1000
#include "make_bignum.h"
int main(int argc, char *argv[])
{

	if ( argc < 2)
	{
		printf("requires a test type, like 'div' or 'sub'\n");
		return 1;
	}
	FILE *fp = fopen("testing.bc", "w+");
	if (fp == NULL)
		return 1;
	char *string1;
	if (strcmp(argv[1], "sqrt") == 0)
		string1 = make_bignum(MAXIMA, 10, 0);
	else
		string1 = make_bignum(MAXIMA, 10, 1);

	if (string1 == NULL)
		return 1;

	size_t scale = random() % MAXIMA;
	fprintf(fp, "scale=%zu;\n", scale);

	if (strcmp(argv[1], "sqrt") == 0)
		fprintf(fp, "sqrt(%s)\nquit\n", string1);

	if (strcmp(argv[1], "div") == 0)
		fprintf(fp, "%s / %s\nquit\n", string1, string1);
	
	if (strcmp(argv[1], "add") == 0)
		fprintf(fp, "%s + %s\nquit\n", string1, string1);
	
	if (strcmp(argv[1], "sub") == 0)
		fprintf(fp, "%s - %s\nquit\n", string1, string1);
	
	if (strcmp(argv[1], "mul") == 0)
		fprintf(fp, "%s * %s\nquit\n", string1, string1);

	fxdpnt *a;
	a = arb_str2fxdpnt(string1);
	
	if (strcmp(argv[1], "div") == 0)
		a = arb_div(a, a, a, 10, scale);
	
	if (strcmp(argv[1], "add") == 0)
		a = arb_add(a, a, a, 10);
	
	if (strcmp(argv[1], "sub") == 0)
		a = arb_sub(a, a, a, 10);
	
	if (strcmp(argv[1], "mul") == 0)
		a = arb_mul(a, a, a, 10, scale);
	
	if (strcmp(argv[1], "sqrt") == 0)
		a = nsqrt(a, 10, scale);
	
	arb_print(a);
	arb_free(a);
	arb_freeglobals();
	free(string1);
	fclose(fp);
	return 0;
}


#include <arbitraire/arbitraire.h>
#include <stdlib.h>
#include <time.h>
#define MAXIMA 10000
#define BASE_MAX 10
#include "make_bignum.h"

int main(int argc, char *argv[])
{
	fxdpnt *a, *b, *c;
	FILE *fp;
	char *string1, *string2;
	size_t scale;
	
	if ( argc < 2)
	{
		printf("requires a test type, like 'div' or 'sub'\n");
		return 1;
	}

	if (!(fp = fopen("testing.bc", "w+")))
		return 1;
	if (!(string1 = make_bignum(MAXIMA, BASE_MAX, 1)))
		return 1;
	if (!(string2 = make_bignum(MAXIMA, BASE_MAX, 1)))
		return 1;
	

	scale = random() % MAXIMA;
	fprintf(fp, "scale=%zu;\n", scale);
	if (strcmp(argv[1], "div") == 0)
		fprintf(fp, "%s / %s\nquit\n", string1, string2);
	
	if (strcmp(argv[1], "newtonian-div") == 0)
		fprintf(fp, "%s / %s\nquit\n", string1, string2);
	
	if (strcmp(argv[1], "add") == 0)
		fprintf(fp, "%s + %s\nquit\n", string1, string2);
	
	if (strcmp(argv[1], "sub") == 0)
		fprintf(fp, "%s - %s\nquit\n", string1, string2);

	if (strcmp(argv[1], "karatsuba-mul") == 0)
		fprintf(fp, "%s * %s\nquit\n", string1, string2);

	if (strcmp(argv[1], "mul") == 0)
		fprintf(fp, "%s * %s\nquit\n", string1, string2);
	
	if (strcmp(argv[1], "mod") == 0)
		fprintf(fp, "%s %% %s\nquit\n", string1, string2);


	if (!(a = arb_str2fxdpnt(string1)))
		return 1;
	if (!(b = arb_str2fxdpnt(string2)))
		return 1;
	if (!(c = arb_expand(NULL, MAXIMA*2)))
		return 1;
	
	if (strcmp(argv[1], "div") == 0)
		c = arb_div(a, b, c, 10, scale);
	
	if (strcmp(argv[1], "newtonian-div") == 0)
		c = arb_newtonian_div(a, b, c, 10, scale);
	
	if (strcmp(argv[1], "add") == 0)
		c = arb_add(a, b, c, 10);
	
	if (strcmp(argv[1], "sub") == 0)
		c = arb_sub(a, b, c, 10);
	
	if (strcmp(argv[1], "mul") == 0)
		c = arb_mul(a, b, c, 10, scale);
	
	if (strcmp(argv[1], "karatsuba-mul") == 0)
		c = arb_karatsuba_mul(a, b, c, 10, scale);

	if (strcmp(argv[1], "mod") == 0)
		c = arb_mod(a, b, c, 10, scale);

	free(string1);
	free(string2);
	arb_print(c);
	arb_free(a);
	arb_free(b);
	arb_free(c);
	fclose(fp);
	arb_freeglobals();

	return 0;
}


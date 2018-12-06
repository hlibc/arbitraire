#include <arbitraire/arbitraire.h>
#include <stdlib.h>
#include <time.h>
#define BASE_MAX 10

int main(int argc, char *argv[])
{
	fxdpnt *a, *b, *c;
	FILE *fp = NULL;
	char *string1, *string2;
	size_t scale = 0;
	int ag = 1;
	size_t maxima = 0;
	int type = 1;

	string1 = string2 = NULL;
	a = b = c = NULL;
	
	if (argc < 4)
	{
		printf("requires a test type, like 'div' or 'sub'\n");
		printf("requires a numerical maximum\n");
		printf("'agnostic' activates a-a-a function mode\n");
		printf("./tests/random-tests div 10000 agnostic\n");
		return 1;
	}

	maxima = strtoll(argv[2], NULL, 10);
	if (strcmp(argv[3], "agnostic") == 0)
		ag = 0;
	if (strcmp(argv[1], "sqrt") == 0)
		type = 0;

	if (!(fp = fopen("testing.bc", "w+")))
		return 1;
	if (!(string1 = make_bignum(maxima, BASE_MAX, type)))
		return 1;
	if (!(string2 = make_bignum(maxima, BASE_MAX, type)))
		return 1;
	

	scale = random() % maxima;
	fprintf(fp, "scale=%zu;\n", scale);

	
	if (!(a = arb_str2fxdpnt(string1)))
		return 1;
	if (!(b = arb_str2fxdpnt(string2)))
		return 1;
	if (!(c = arb_expand(NULL, maxima*2)))
		return 1;

	
	if (strcmp(argv[1], "sqrt") == 0) {
		fprintf(fp, "sqrt(%s)\nquit\n", string1);
		a = nsqrt(a, 10, scale);
		arb_print(a);
	}

	if (strcmp(argv[1], "div") == 0) {
		if (ag) {
			fprintf(fp, "%s / %s\nquit\n", string1, string2);
			c = arb_div(a, b, c, 10, scale);
			arb_print(c);
		} else {
			fprintf(fp, "%s / %s\nquit\n", string1, string1);
			a = arb_div(a, a, a, 10, scale);
			arb_print(a);
		}
	}
	
	if (strcmp(argv[1], "newtonian-div") == 0) {
		if (ag) {
			fprintf(fp, "%s / %s\nquit\n", string1, string2);
			c = arb_newtonian_div(a, b, c, 10, scale);
			arb_print(c);
		} else { 
			fprintf(fp, "%s / %s\nquit\n", string1, string1);
			a = arb_newtonian_div(a, a, a, 10, scale);
			arb_print(a);
		}
	}
	
	if (strcmp(argv[1], "add") == 0) {
		if (ag) {
			fprintf(fp, "%s + %s\nquit\n", string1, string2);
			c = arb_add(a, b, c, 10);
			arb_print(c);
		} else { 
			fprintf(fp, "%s + %s\nquit\n", string1, string1);
			a = arb_add(a, a, a, 10);
			arb_print(a);
		}
	}
	
	if (strcmp(argv[1], "sub") == 0) {
		if (ag) {
			fprintf(fp, "%s - %s\nquit\n", string1, string2);
			c = arb_sub(a, b, c, 10);
			arb_print(c);
		} else { 
			fprintf(fp, "%s - %s\nquit\n", string1, string1);
			a = arb_sub(a, a, a, 10);
			arb_print(a);
		}
	}

	if (strcmp(argv[1], "mul") == 0) {
		if (ag) {
			fprintf(fp, "%s * %s\nquit\n", string1, string2);
			c = arb_mul(a, b, c, 10, scale);
			arb_print(c);
		} else { 
			fprintf(fp, "%s * %s\nquit\n", string1, string1);
			a = arb_mul(a, a, a, 10, scale);
			arb_print(a);
		}
	}
	
	if (strcmp(argv[1], "mod") == 0) {
		if (ag) {
			fprintf(fp, "%s %% %s\nquit\n", string1, string2);
			c = arb_mod(a, b, c, 10, scale);
			arb_print(c);
		} else { 
			fprintf(fp, "%s %% %s\nquit\n", string1, string1);
			a = arb_mod(a, a, a, 10, scale);
			arb_print(a);
		}
	}

	free(string1);
	free(string2);
	arb_free(a);
	arb_free(b);
	arb_free(c);
	fclose(fp);

	return 0;
}


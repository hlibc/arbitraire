#include <arbitraire/arbitraire.h>

int main(int argc, char *argv[])
{
	if (argc < 4 )
		arb_error("Needs 3 args, such as: 123 123 base");
	
	int base = strtoll(argv[3], NULL, 10);
	fxdpnt *a, *b;
	a = arb_str2fxdpnt(argv[1]);
	b = arb_str2fxdpnt(argv[2]);
	int c = arb_compare(a, b, base);
	if (c > 0) {
		fprintf(stdout, ">\n");
	} else if (c < 0) {
		fprintf(stdout, "<\n");
	} else {
		fprintf(stdout, "==\n");
	}
	arb_free(a);
	arb_free(b);

	return 0;
}


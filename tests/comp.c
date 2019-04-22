#include <arbitraire/arbitraire.h>

int main(int argc, char *argv[])
{
	if (argc < 3) {
		arb_error("Needs 2 args, such as: 123 123");
	}

	fxdpnt *a, *b;
	a = arb_str2fxdpnt(argv[1]);
	b = arb_str2fxdpnt(argv[2]);
	int c = arb_compare(a, b);
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


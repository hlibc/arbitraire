#include <arbitraire/arbitraire.h>

int main(int argc, char *argv[])
{
	if (argc < 2) {
		arb_error("Needs 1 arg, such as: 123.123");
	}

	fxdpnt *a, *b;
	a = arb_str2fxdpnt(argv[1]);
	b = NULL;
	b = arb_copy(b, a);
	arb_print(a);
	arb_attrs(a, 0);
	arb_print(b);
	arb_attrs(b, 0);
	arb_free(a);
	arb_free(b);

	return 0;
}


#include <arbitraire/arbitraire.h>

int main(int argc, char *argv[])
{
	if (argc < 5 )
		arb_error("Needs 4 args, such as: 123 123 base scale");


	int base = atoll(argv[3]);
	int scale = atoll(argv[4]);

	fxdpnt *a, *b, *c = NULL;
	a = arb_str2fxdpnt(argv[1]);
	b = arb_str2fxdpnt(argv[2]);
	c = arb_mul(a, b, c, base, scale);
	arb_print(c);
	arb_free(a);
	arb_free(b);
	arb_free(c);
	return 0;
}


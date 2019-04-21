#include <arbitraire/arbitraire.h>

int main(int argc, char *argv[])
{
	if (argc < 2 )
		arb_error("Needs 1 arg, such as: .0000123");

	fxdpnt *a = NULL;
	a = arb_str2fxdpnt(argv[1]);
	size_t len = count_leading_fractional_zeros(a);
	printf("number of fractional zeros was %zu\n", len);
	arb_free(a);
	return 0;
}


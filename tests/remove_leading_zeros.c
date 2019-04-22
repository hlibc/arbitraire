#include <arbitraire/arbitraire.h>

int main(int argc, char *argv[])
{
	if (argc < 2 )
		arb_error("Needs 1 arg, such as: 000123.0000123");

	fxdpnt *a = NULL;
	a = arb_str2fxdpnt(argv[1]);
	a = remove_leading_zeros(a);
	arb_print(a);
	arb_free(a);
	return 0;
}


#include <arbitraire/arbitraire.h>

int main(int argc, char *argv[])
{
	if (argc < 5 )
		arb_error("Needs 4 args, such as: 123 123 base scale");
	
	int base = strtoll(argv[3], NULL, 10);
	int scale = strtoll(argv[4], NULL, 10);
	fxdpnt *a;
	a = arb_str2fxdpnt(argv[1]);
	a = arb_mul(a, a, a, base, scale);
	arb_print(a);
	arb_free(a);
	return 0;
}


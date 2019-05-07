#include <arbitraire/arbitraire.h>

int main(int argc, char *argv[])
{
	if (argc < 5)
	{
		printf("needs 4 args: bignum bignum base scale\n");
		return 0;
	}

	int base = strtoll(argv[3], NULL, 10);
	int scale =  strtoll(argv[4], NULL, 10);
	fxdpnt *a = NULL;
	fxdpnt *b = NULL;
	fxdpnt *c = NULL;
	fxdpnt *override = NULL;
	arb_expand(c, 1);
	a = arb_str2fxdpnt(argv[1]);
	b = arb_str2fxdpnt(argv[2]);
	if (argv[5])
	override = arb_str2fxdpnt(argv[5]);

	c = arb_newtonian_div(a, b, c, base, scale, override);
	arb_print(c);
	return 0;
}
 

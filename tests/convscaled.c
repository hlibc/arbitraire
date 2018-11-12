#include <arbitraire/arbitraire.h>

int main(int argc, char *argv[])
{
	if (argc < 4)
	{
		printf("needs 3 args: bignum ibase obase\n");
		return 0;
	}
	
	int ibase = strtoll(argv[2], NULL, 10);
	int obase = strtoll(argv[3], NULL, 10);
	fxdpnt *a = NULL;
	fxdpnt *b = NULL;
	a = arb_expand(a, 1);
	b = arb_expand(b, 1);
	a = arb_str2fxdpnt(argv[1]);
	b = convscaled(a, b, ibase, obase);
	arb_print(b);
	arb_free(a);
	arb_free(b);
	return 0;
}


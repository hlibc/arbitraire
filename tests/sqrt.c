#include <arbitraire/arbitraire.h>

int main(int argc, char *argv[])
{
	if (argc < 4)
	{
		printf("needs 3 args: bignum base scale\n");
		return 0;
	}

	int base = atoll(argv[2]);
	int scale =  atoll(argv[3]);
	fxdpnt *a = NULL;
	//fxdpnt *c = NULL;
	//c = arb_expand(c, 10000);
	a = arb_str2fxdpnt(argv[1]);
	a = nsqrt(a, base, scale);
	arb_print(a);
	return 0;
}


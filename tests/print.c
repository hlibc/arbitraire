#include <arbitraire/arbitraire.h>

int main(int argc, char *argv[])
{
	if (argc < 2 )
		arb_error("Needs a bignum like: 123.213");

	fxdpnt *a;

	a = arb_str2fxdpnt(argv[1]);

	printf("%s\n", argv[1]);
	arb_print(a);
	printf("->len = %zu\n", a->len);
	printf("->lp = %zu\n", a->lp);
	printf("->allocated = %zu\n", a->allocated);
	printf("->sign = %c\n", a->sign);

	arb_free(a);
	return 0;
}


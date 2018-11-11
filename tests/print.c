#include <arbitraire/arbitraire.h>

int main(int argc, char *argv[])
{
	if (argc < 2 )
		arb_error("Needs a bignum like: 123.213");

	fxdpnt *a;

	a = arb_str2fxdpnt(argv[1]);

	printf("%s\n", argv[1]);
	arb_print(a);
	printf("->len = %zu\n", arb_size(a));
	printf("->lp = %zu\n", arb_left(a));
	printf("->allocated = %zu\n", arb_allocated(a));
	printf("->sign = %c\n", arb_sign(a));

	arb_free(a);
	return 0;
}


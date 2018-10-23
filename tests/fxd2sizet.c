#include <arbitraire/arbitraire.h>

int main(int argc, char *argv[])
{
	if (argc < 2)
		arb_error("Needs 1 numerical arg");

	fxdpnt *a = arb_str2fxdpnt(argv[1]);
	size_t ret = fxd2sizet(a, 10);
	printf("%zu\n", ret);
	return 0;
}


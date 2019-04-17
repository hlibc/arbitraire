#include <arbitraire/arbitraire.h>

int main(int argc, char *argv[])
{ 
	fxdpnt *a;
	if (argc < 2) {
		fprintf(stderr, "Usage: %s bignum num_to_shift\n", argv[0]);
		return 1;
	}
	a = arb_str2fxdpnt(argv[1]);
	size_t n = strtol(argv[2], 0, 0);

	arb_rightshift(a, n);
	arb_print(a); 
	arb_free(a);
	return 0;
}


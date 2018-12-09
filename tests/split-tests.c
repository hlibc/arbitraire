#include <arbitraire/arbitraire.h>

int main(int argc, char *argv[])
{ 
	if (argc < 3) {
		fprintf(stderr, "requires two karatsuba numbers to split\n");
		return 1;
	}
	fxdpnt *a, *b = NULL;
	a = arb_str2fxdpnt(argv[1]);
	b = arb_str2fxdpnt(argv[2]);

	split_test(a, b); 
	return 0;
}


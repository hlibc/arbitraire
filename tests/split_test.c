#include <arbitraire/arbitraire.h>

int main(int argc, char *argv[])
{ 
	fxdpnt *a, *b = NULL;
	a = arb_str2fxdpnt(argv[1]);
	b = arb_str2fxdpnt(argv[2]);

	split_test(a, b); 
	return 0;
}


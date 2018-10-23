#include <arbitraire/arbitraire.h>

int main(int argc, char *argv[])
{
	if (argc < 4 )
		arb_error("Needs 3 args, such as:  123 / 123 base");
 
	
	int base = atoll(argv[3]);
	fxdpnt *a, *b, *c = NULL;
	a = arb_str2fxdpnt(argv[1]);
	b = arb_str2fxdpnt(argv[2]);
	c = arb_sub(a, b, c, base);
	arb_print(c);
	arb_free(a);
	arb_free(b);
	arb_free(c);
	return 0;
}


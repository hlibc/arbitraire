#include <arbitraire/arbitraire.h>
void split_test(fxdpnt *a, fxdpnt *b)
{
        fxdpnt *aa = NULL;
        fxdpnt *bb = NULL;
        fxdpnt *cc = NULL;
        fxdpnt *dd = NULL;
        size_t comp = split(a, b, &aa, &bb, &cc, &dd);
        printf("compensated magnitude %zu\n", comp);
        arb_printtrue(a);
        arb_printtrue(b);
        arb_printtrue(aa);
        arb_printtrue(bb);
        arb_printtrue(cc);
        arb_printtrue(dd);
}

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


#include "internal.h"

void arb_attrs(fxdpnt *a, char *m)
{
	printf("%s->len = %zu\n", m, arb_size(a));
        printf("%s->lp = %zu\n", m, arb_left(a));
        printf("%s->allocated = %zu\n", m, arb_allocated(a));
        printf("%s->sign = %c\n", m, arb_sign(a));
}

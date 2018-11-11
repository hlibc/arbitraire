#include "internal.h"

char *make_bignum(size_t limit, int base, int tryneg)
{
        static int set = 0;
        if (set == 0)
        {
                srandom(time(NULL));
                set = 1;
        }
        size_t truelim = random() % limit;
        while (truelim < 2)
                truelim = random() % limit;
        char *ret;
        size_t i = 0;
        int sign;
        if (!(ret = malloc(truelim + 1)))
                return NULL;
        if (tryneg)
        {
                sign = (random() % 2);
                if (sign == 0)
                        ret[i++] = '-';
        }
        for(;i < truelim; i++)
                ret[i] = arb_highbase((random() % base));

        ret[(random() % truelim)] = '.';
        ret[i] = 0;
        return ret;
}

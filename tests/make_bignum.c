#include <arbitraire/arbitraire.h>

int main(void)
{
	char *s = make_bignum(1000, 10, 1);
	size_t len = strlen(s);
	size_t waste = 0; // suppress meaningless -O3 write() retval warnings
	waste = write(1, s, len);
	waste += write(1, "\n", 1);
	printf("length of array was %zu\n", len);
	fflush(stdout);
	return 0;
}

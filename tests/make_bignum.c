#include <arbitraire/arbitraire.h>

int main(void)
{
	char *s = make_bignum(3, 10, 1);
	size_t len = strlen(s);
	write(1, s, len);
	write(1, "\n", 1);
	printf("length of array was %zu\n", len);
	fflush(stdout);
	return 0;
}

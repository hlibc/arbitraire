#include <arbitraire/arbitraire.h>

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "requires a number to test for oddity\n");
		return 1;
	}

	int num = strtoll(argv[1], NULL, 10);
	
	if (oddity(num))
		printf("was odd\n");
	else
		printf("was even\n");

	return 0;
}


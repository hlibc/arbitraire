#include <arbitraire/arbitraire.h>

int main(int argc, char *argv[])
{




	int num = strtoll(argv[1], NULL, 10);
	
	if (oddity(num))
		printf("was odd\n");
	else
		printf("was even\n");

	return 0;
}


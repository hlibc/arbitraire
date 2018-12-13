#include <arbitraire/arbitraire.h>
#include <ctype.h>
int getword(char *word, size_t lim, FILE *fp)
{
	int c;
	char *w = word;
	while (isspace(c = fgetc(fp)));
		if (c != EOF)
			*w++ = c;
	if (!isalnum(c) && !ispunct(c))
	{
		*w = '\0';
		return c;
	}
	for ( ; --lim > 0; w++)
	{
		*w = fgetc(fp);
		if (!isalnum(*w) && !ispunct(*w))
		{
			ungetc(*w, fp);
			break;
		}
	}
	*w = '\0';
	return word[0];
}

int main(int argc, char *argv[])
{
	FILE *o = fopen(argv[1], "r");
	char c = 0;
	char *str = malloc(1000000);
	size_t scale = 0;
	char *string1 = NULL;
	char *string2 = NULL;
	int division = 0;
	int multiplication = 0;
	int subtraction = 0;
	int modulus = 0;
	int addition = 0;
	while (getword(str,1000000, o)!=EOF)
	{
		
		if (strcmp(str, "scale") == 0)
		{
			getword(str,1000000, o);
			if (strcmp(str, "=") == 0)
			{
				getword(str,1000000, o);
				scale = strtol(str, NULL, 10);
			}
			getword(str,1000000, o);
		}
		if (strcmp(str, ";"))
			;
		if (strcmp(str, "/") == 0)
			division = 1;
		if (isdigit(*str) && string1 == NULL)
			string1 = strdup(str);
		else if (isdigit(*str) && string2 == NULL)
			string2 = strdup(str);
		printf("%s\n", str);
	}
	if (division)
		printf("was division\n");
	printf("scale was %zu\n", scale);
	printf("string1 was %s\n", string1);
	printf("string2 was %s\n", string2);	
	return 0;
}

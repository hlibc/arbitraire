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
	while (getword(str,1000000, o)!=EOF)
	{
		
		if (strcmp(str, "scale") == 0)
		{
			scale = strtol(str, NULL, 10);
		}
		printf("%s\n", str);
	}
	return 0;
}

#include "internal.h"


int arb_highbase(int a)
{
	/* Handle high bases */
	static int glph[36] = { '0', '1', '2', '3', '4', '5', '6', '7', '8',
				'9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
				'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
				'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
	if (a < 36) 
		return glph[a]; 
	else /* just use the ascii values for bases that are very high */
		return a;
}

void _print_core(FILE *fp, UARBT *number, size_t len, size_t radix, size_t sign, int fold)
{
	size_t i = 0;
	size_t k = sign;

	for (i=0; i < len ; ++i){

		if (fold && k % 68 == 0 && k != 0) {
			fputc('\\', fp);
			fputc('\n', fp);
		}
			

		if (radix == i)
		{
			fprintf(fp, "."); 
			++k;
			if (fold && k % 68 == 0 && k != 0) {
				fputc(arb_highbase((number[i])), fp);
				fputc('\\', fp);
				fputc('\n', fp);
			}
				
		}
		
		fputc(arb_highbase((number[i])), fp);
		++k;
	}

	if (!len)
		fputc('0', fp); 
	
	fputc('\n', fp); 

	fflush(fp);
}

void arb_fprint(FILE *fp, fxdpnt *flt)
{
	size_t sign = 0;
	
	if (iszero(flt) == 0)
	{
		fputc('0', fp);
		fputc('\n', fp);
		goto end;
	}
	if (flt->sign == '-')
	{
		//putchar(flt->sign);
		fputc(flt->sign, fp);
		sign = 1;
	}
	_print_core(fp, flt->number, flt->len, flt->lp, sign, 1);
	end:
	fflush(fp);
}

void arb_print(fxdpnt *flt)
{ 
	if (flt == NULL)
	{
		fprintf(stdout, "number was (null)\n");
		return;
	}
	arb_fprint(stdout, flt);
}

void arb_printerr(fxdpnt *flt)
{
	if (flt == NULL)
	{
		fprintf(stdout, "number was (null)\n");
		return;
	}
	arb_fprint(stderr, flt);
}

void arb_printtrue(fxdpnt *flt)
{
	/*
		This function prints fxdpnt's as they truly are as opposed
		to jostling them for correct output
	*/

	size_t k = 0;
	if (flt->sign == '-') {
		fputc(flt->sign, stdout);
	}
	
	while (k < flt->len) {
		fputc(arb_highbase((flt->number[k])), stdout);
                ++k;
        }
}


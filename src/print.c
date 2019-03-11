#include "internal.h"

/* Copyright 2017-2019 CM Graff */

/* Various routines for printing a fxdpnt bignum.
 * POSIX `bc' line splitting at 80 columns is used. For a non-split number 
 * use arb_printtrue.
*/

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

void _print_core(FILE *fp, const UARBT *number, size_t len, size_t radix, size_t sign)
{
	size_t i = 0;
	size_t k = sign; /* set to 1 or 0 -- to account for the sign */

	for (; i < len ; ++i, ++k) {
		if (k != 0 && k % 68 == 0) {
			fputc('\\', fp);
			fputc('\n', fp);
		}
		if (radix == i) {
			fputc('.', fp);
			if ((++k) % 68 == 0) {
				fputc('\\', fp);
				fputc('\n', fp);
			}
		}
		fputc(arb_highbase((number[i])), fp);
	}

	if (!len) {
		fputc('0', fp);
	}

	fputc('\n', fp);

	fflush(fp);
}

void arb_fprint(FILE *fp, const fxdpnt *flt)
{
	if (flt == NULL) {
		fputs("number was (null)\n", fp);
		return;
	}
	if (iszero(flt) == 0) {
		fputc('0', fp);
		fputc('\n', fp);
		goto end;
	}
	if (flt->sign == '-') {
		fputc(flt->sign, fp);
		_print_core(fp, flt->number, flt->len, flt->lp, 1);
	}
	else {
		_print_core(fp, flt->number, flt->len, flt->lp, 0);
	}
	end:
	fflush(fp);
}

void arb_print(const fxdpnt *flt)
{ 
	arb_fprint(stdout, flt);
}

void arb_printerr(const fxdpnt *flt)
{
	arb_fprint(stderr, flt);
}

void arb_printtrue(const fxdpnt *flt)
{
	size_t k = 0;
	if (flt->sign == '-') {
		fputc(flt->sign, stdout);
	}
	
	for(;k < flt->len;++k) {
		fputc(arb_highbase((flt->number[k])), stdout);
        }
	fputc('\n', stdout);
}


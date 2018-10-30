#include <assert.h>
#include <string.h>
#include <arbitraire/arbitraire.h>

void arb_error(char *message)
{
	fprintf(stderr, "%s\n", message);
	exit(1);
}

size_t rr(fxdpnt *a)
{
	return a->len - a->lp;
}

void arb_free(fxdpnt *flt)
{
	arb_destroy(flt);
	free(flt);
}

void arb_destroy(fxdpnt *flt)
{
	if (flt->number) {
		free(flt->number);
		flt->number = NULL;
		flt->allocated = 0;
	}
}

void arb_init(fxdpnt *flt)
{
	flt->sign = '+';
	flt->len = 0;
}

fxdpnt *arb_create(fxdpnt *flt, size_t len)
{
	if (!flt) {
		flt = arb_malloc(sizeof(fxdpnt));
	}

	arb_init(flt);
	flt->number = arb_calloc(1, sizeof(ARBT) * len);
	flt->allocated = len;
	flt->len = len;
	flt->lp = 0;
	return flt;
}

void *arb_malloc(size_t len)
{
	void *ret;
	if(!(ret = malloc(len)))
		arb_error("arb_malloc (malloc) failed\n");
	return ret;
}

void *arb_calloc(size_t nmemb, size_t len)
{
	void *ret;
	if(!(ret = calloc(nmemb, len)))
		arb_error("arb_calloc (calloc) failed\n");
	return ret;
}

fxdpnt *arb_alloc(size_t len)
{
	return arb_create(NULL, len);
}

void *arb_realloc(void *ptr, size_t len)
{
	void *ret;
	if(!(ret = realloc(ptr, len)))
		arb_error("realloc failed\n"); // FIXME: return an error, and inform the user
	return ret;
}

fxdpnt *arb_expand(fxdpnt *flt, size_t request)
{
	static int lever = 0;

	/* align on a multiple of 2 */
	if (request > 16)
		request = (((request / 16) + 1) * 16);
	else
		request = 16;
	
	if (flt == NULL) {
		flt = arb_alloc(request);
	} else if (request > flt->allocated) {
		flt->allocated = request;
		flt->number = arb_realloc(flt->number, flt->allocated * sizeof(ARBT));
		memset(flt->number + flt->len, 0, (flt->allocated - flt->len) * sizeof(ARBT));
	}
	if (lever == 0)
	{
		lever = 1;
		zero = arb_str2fxdpnt("0");
		p5 = arb_str2fxdpnt("0.5");
		one = arb_str2fxdpnt("1");
		two = arb_str2fxdpnt("2");
		ten = arb_str2fxdpnt("10");
	}
	return flt;
}

#include "internal.h"

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
	if (flt->number) {
		free(flt->number);
		flt->number = NULL;
		flt->allocated = 0;
	}
	free(flt);
}

void arb_init(fxdpnt *flt)
{
	flt->sign = '+';
	flt->len = 0;
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

void *arb_realloc(void *ptr, size_t len)
{
	void *ret;
	if(!(ret = realloc(ptr, len)))
		arb_error("realloc failed\n"); // FIXME: return an error, and inform the user
	return ret;
}

void arb_cleanup(void)
{
	if (zero)
		arb_free(zero);
	if (p5)
		arb_free(p5);
	if (one)
		arb_free(one);
	if (two)
		arb_free(two);
	if (ten)
		arb_free(ten);
}

fxdpnt *arb_expand(fxdpnt *o, size_t request)
{
	static int lever = 0;

	/* align on a multiple of 16 */

	if (request > 16)
		request = (((request / 16) + 1) * 16);
	else
		request = 16;

	
	if (o == NULL) { 
		o = arb_malloc(sizeof(fxdpnt));
		arb_init(o);
		o->number = arb_calloc(1, sizeof(UARBT) * request);
		o->allocated = request;
		o->len = request;
		o->lp = 0;
	} else if (request > o->allocated) {
		o->allocated = request;
		o->number = arb_realloc(o->number, o->allocated * sizeof(UARBT));
		_arb_memset(o->number + o->len, 0, o->allocated - o->len);
	}
	if (lever == 0)
	{
		lever = 1;
		zero = arb_str2fxdpnt("0");
		p5 = arb_str2fxdpnt("0.5");
		one = arb_str2fxdpnt("1");
		two = arb_str2fxdpnt("2");
		ten = arb_str2fxdpnt("10");
		atexit(arb_cleanup);
	}
	return o;
}

size_t arb_size(fxdpnt *a)
{
        return a->len;
}

size_t arb_allocated(fxdpnt *a)
{
        return a->allocated;
}

char arb_sign(fxdpnt *a)
{
        return a->sign;
}

size_t arb_left(fxdpnt *a)
{
	return a->lp;
}

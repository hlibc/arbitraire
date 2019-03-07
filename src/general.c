#include "internal.h"

void arb_error(char *message)
{
	/* arbitraire exits upon memory exhaustion. This is not ideal, but it
	   saves many lines of codes versus returning the error back to the
	   caller. At some future release this should be changed to return the 
	   error to the caller instead of calling exit().
	*/
	fprintf(stderr, "%s\n", message);
	exit(1);
}

/* wrappers to expose the length of the number on either side of the radix */
size_t rl(const fxdpnt *a)
{
	return a->lp;
}

size_t rr(const fxdpnt *a)
{
	return a->len - a->lp;
}

size_t le(const fxdpnt *a)
{
	return a->len;
}

/* memory management and bignum creation routines */
void arb_free(fxdpnt *flt)
{
	if (flt && flt->number) {
		free(flt->number);
		/* sanitize the memory */
		flt->allocated = 0;
		flt->len = 0;
		flt->lp = 0;
		flt->sign = 0;
	}
	if (flt)
		free(flt);
}

void arb_init(fxdpnt *flt)
{
	flt->sign = '+';
	flt->len = 0;
	flt->lp = 0;
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
		arb_error("arb_realloc (realloc) failed\n");
	return ret;
}

static void arb_cleanup(void)
{
	/* this function is called by atexit() and should never be
	   invoked manually */
	arb_free(zero);
	arb_free(p5);
	arb_free(one);
	arb_free(two);
	arb_free(three);
	arb_free(ten);
}

fxdpnt *arb_expand_inter(fxdpnt *o, size_t request, size_t left)
{
	static int lever = 0;
	size_t original = request;

	/* align on a multiple of 16 */
	if (request > 16)
		request = (((request / 16) + 1) * 16);
	else
		request = 16;

	/* allocation (vector creation) */
	if (o == NULL) { 
		o = arb_malloc(sizeof(fxdpnt));
		arb_init(o);
		o->number = arb_calloc(1, sizeof(UARBT) * request);
		o->allocated = request;
		o->lp = o->len = original;
	/* reallocation (vector expansion) */
	} else if (request > o->allocated) {
		o->allocated = request;
		o->number = arb_realloc(o->number, o->allocated * sizeof(UARBT));
		_arb_memset(o->number + o->len, 0, o->allocated - o->len);
	}

	/* initialize the global constants (once) */
	if (lever == 0) {
		lever = 1;
		zero = arb_str2fxdpnt("0");
		p5 = arb_str2fxdpnt("0.5");
		one = arb_str2fxdpnt("1");
		two = arb_str2fxdpnt("2");
		three = arb_str2fxdpnt("3");
		ten = arb_str2fxdpnt("10");
		atexit(arb_cleanup);
	}

	/* allow specific radix positioning requests */
	if (left) {
		o->lp = left;
		o->len = original;
	}

	return o;
}

fxdpnt *arb_expand(fxdpnt *o, size_t request)
{
	return arb_expand_inter(o, request, 0);
}

/* simple opacity wrappers so that the caller can display debug attributes */
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

void arb_attrs(fxdpnt *a, char *m)
{
	printf("%s->len = %zu\n", m, arb_size(a));
        printf("%s->lp = %zu\n", m, arb_left(a));
        printf("%s->allocated = %zu\n", m, arb_allocated(a));
        printf("%s->sign = %c\n", m, arb_sign(a));
}

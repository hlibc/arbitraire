#include "internal.h"
/* Copyright 2017-2019 CM Graff */


/*
	make_bignum() is designed to make bignums that test some of the worst
	scenarios of a number system

	1> Ergo, horrible numbers like the following are sometimes generated:

		.0
		-0.
		123. 
		.123
		-.123
		000123123.123123
		-00.00

	2> As well as nicely formed numbers such as:

		123.123
		-123
		-0.123
		38672384768923746872346.2346782364786234
		.0000000123012

		Millions of randomly generated number sequences seldom
		result in the harder tests which generally must be hand-
		crafted. One of such sequence types is the case of
		leading fractional zeros like .0000xxxx. For this reason,
		make_bignum will attempt to make a true fractional sequence
		with no whole number part and many many leading zeros about
		20 percent of the time.

		TODO: create numbers with huge expanses of zeros such as
		123.0000000....0000123

	3> Invalid numbers are also generated. Such as:
		-.

		Testing reveals that -. and . are both treated as a zero by
		arbitraire. Which is probably good. Some of arbitraire's
		effectiveness is refined by it treating .0, 0.0 and 0. all
		the same way. This method drastically reduces the "fiddly"
		types of code which have the tendency to create bugs in
		mathematics systems due to them requiring nudging and 
		testing of properties which arbitraire treats naturally.

	4> single digit numbers are not formed. 
		4
		6

	Points 1> and 2> are good, point 3> may be useful to some extent
	and should likely (but doesn't) produce a warning from arbitraire.

	Testing reveals that -. and . are treated as a positive zero by
	arbitraire.

	Arbitraire does not honor -0. It appears that some other implementations
	also do not honor -0.

		prompt> echo "3 * -0"| bc -l
		prompt> 0

	I am not sure what the formal rule for this is, but I suspect it would 
	be ideal to honor -0 and flip the sign accordingly.


	Point 4> should be fixed. But not at the cost of sacrificing 1>, 2>
	or 3>. Obviously, single numbers are an important case, but far from the
	*most* important and such a failure would be a serious and unacceptable
	regression.

	make_bignum requires 3 arguments, the limit, which in most cases should
	be between 1000 and 10000, (but can be no smaller than 3), the base, and
	finally an option for *not* trying negative numbers.

	If a limit argument of less than 3 is passed to make_bignum the behavior
	is undefined.
*/

char *make_bignum(size_t limit, int base, int tryneg)
{
	static int set = 0;
	char *ret = NULL;
	size_t i = 0;
	int sign = 0;
	size_t truelim = 0;

	if (set == 0)
	{
		srandom(time(NULL));
		set = 1;
	}
	truelim = random() % limit;
	while (truelim < 2)
		truelim = random() % limit;
	if (!(ret = malloc(truelim + 1)))
		return NULL;
	if (tryneg)
	{
		sign = (random() % 2);
		if (sign == 0)
			ret[i++] = '-';
	}
	/* try fractions only */
	if (random() % 4 == 2)
	{
		size_t zeros = random() % (truelim /2);
		if (random() % 4 == 2)
			ret[i++] = '0';
		ret[i++] = '.';
		while (zeros--)
			ret[i++] = '0';

		for(;i < truelim; i++)
			ret[i] = arb_highbase((random() % base));
	/* numbers that will typically have whole-number values */
	} else {
		for(;i < truelim; i++)
			ret[i] = arb_highbase((random() % base));
		/* 1 out of ten nums with whole-parts will be integers */
		if (random() % 10 < 8)
			ret[(random() % truelim)] = '.';
	}
	ret[i] = 0;
	return ret;
}

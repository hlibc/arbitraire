#!/bin/sh

set -e

. ./.config

if [ "$#" -lt 3 ]
then	fatal "Usage: ./tests/random-wrapper.sh div|sub maxima agnostic|null strace|valgrind|time [NUMBER OF TESTS]"
fi

USE_VALGRIND="0"
USE_STRACE="0"
USE_TIME="0"
if [ $# = "4" ]
then	[ "$4" = "valgrind" ] && USE_VALGRIND="1"
	[ "$4" = "strace" ] && USE_STRACE="1"
	[ "$4" = "time" ] && USE_TIME="1"
fi

machinename="$(uname -m)"
machinename="${machinename}-tests-passed.txt"

NUMTESTS="100"
if [ $# = "5" ]
then	NUMTESTS="$5"
fi

stdout "This is a set of PRNG tests to ensure that arbitraire works" >>"${machinename}"
stdout "properly on the following machine type: ${machinename}" >>"${machinename}"
stdout >> "${machinename}"
uname -a >> "${machinename}"
stdout "${machinename}" >> "${machinename}"

stdout "The tests should halt upon detecting an error and the contents" >>"${machinename}"
stdout "of 'testing.bc' can be inspected to reveal the failing test" >>"${machinename}"
stdout >> "${machinename}"
stdout >> "${machinename}"
while [ "$NUMTESTS" -gt "0" ]
do	NUMTESTS="$((NUMTESTS - 1))"
	stdout "Test number: ${COUNT}" >>"${machinename}"
	if [ "$USE_VALGRIND" = "1" ]
	then	valgrind --leak-check=full ./tests/random-tests "$1" "$2" "$3" >log 2>log3
		bc -l testing.bc > log2 2>log4
	elif [ "$USE_STRACE" = "1" ]
	then	strace ./tests/random-tests "$1" "$2" "$3" >log 2>log3
		bc -l testing.bc > log2 2>log4
	elif [ "$USE_TIME" = "1" ]
	then	time ./tests/random-tests "$1" "$2" "$3" >log 2>log3
		time bc -l testing.bc > log2 2>log4
	else	./tests/random-tests "$1" "$2" "$3" >log 2>log3
		bc -l testing.bc > log2 2>log4
	fi
	truecat testing.bc >> "${machinename}"
	if diff log log2
	then	stdout "PRNG driven test passed."
		stdout "PRNG driven number test passed." >> "${machinename}"
	else	stdout "Test failed! look inside of '${machinename}' to reveal the failing numbers!" 
		fatal  "Test failed! look inside of '${machinename}' to reveal the failing numbers!" >> "${machinename}"
	fi
	stdout "stderr log of the test subject" >> "${machinename}"
	truecat log3 >> "${machinename}"
	stdout "stderr log of the control" >> "${machinename}"
	truecat log4 >> "${machinename}"
	stdout >> "${machinename}"
	stdout >> "${machinename}"
	sleep 1 # we need 1 second in order to get a new random number
done


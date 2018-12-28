#!/bin/sh

set -e

. ./.config

if [ "$#" -lt 3 ]
then	fatal "Usage: ./tests/random-wrapper.sh div|sub maxima agnostic|null"
fi

USE_VALGRIND="0"
USE_STRACE="0"
USE_TIME="0"
if [ $# = "4" ]
then	[ "$4" = "valgrind" ] && USE_VALGRIND="1"
	[ "$4" = "strace" ] && USE_STRACE="1"
	[ "$4" = "time" ] && USE_TIME="1"
fi

COUNT="0"
VERSION=$(testcat .version)

machinename="$(gcc -dumpmachine)"
machinename="${VERSION}-${machinename}-tests-passed.txt"

stdout "This is a set of PRNG tests to ensure that arbitraire ${VERSION} works" >>"${machinename}"
stdout "properly on the following machine type:" >>"${machinename}"
stdout >> "${machinename}"
uname -a >> "${machinename}"
stdout "${machinename}" >> "${machinename}"

stdout "The tests should halt upon detecting an error and the contents" >>"${machinename}"
stdout "of 'testing.bc' can be inspected to reveal the failing test" >>"${machinename}"
stdout >> "${machinename}"
stdout >> "${machinename}"
while [ "$COUNT" -lt 100 ]
do	COUNT="$((COUNT + 1))"
	stdout "Test number: ${COUNT}" >>"${machinename}"
	if [ "$USE_VALGRIND" = "1" ]
	then	valgrind --leak-check=full ./tests/random-tests "$1" "$2" "$3" >log 2>log3
	elif [ "$USE_STRACE" = "1" ]
	then	strace ./tests/random-tests "$1" "$2" "$3" >log 2>log3
	elif [ "$USE_TIME" = "1" ]
	then	time ./tests/random-tests "$1" "$2" "$3" >log 2>log3
	else	./tests/random-tests "$1" "$2" "$3" >log 2>log3
	fi
	
	bc -l testing.bc > log2
	testcat testing.bc >> "${machinename}"
	if diff log log2
	then	stdout "Giant pseudo-random number test passed."
		stdout "Giant pseudo-random number test passed." >> "${machinename}"
	else	stdout "Test failed! look inside of '${machinename}' to reveal the failing numbers!" 
		fatal  "Test failed! look inside of '${machinename}' to reveal the failing numbers!" >> "${machinename}"
	fi
	testcat log3 >> "${machinename}"
	stdout >> "${machinename}"
	stdout >> "${machinename}"
	sleep 1 # we need 1 second in order to get a new random number
done


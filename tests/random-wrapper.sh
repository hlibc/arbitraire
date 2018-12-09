#!/bin/sh

set -e

if [ "$#" -lt 3 ]
then	printf "Requires a test type, like 'div', 'add', 'sub' or 'mul'\n"
	printf "Requires a maxima\m"
	printf "Must specify 'agnostic' or not\n"
	exit 1
fi

USE_VALGRIND="0"
USE_STRACE="0"
USE_TIME="0"

[ "$4" = "valgrind" ] && USE_VALGRIND="1"
[ "$4" = "strace" ] && USE_STRACE="1"
[ "$4" = "time" ] && USE_TIME="1"


COUNT="0"

VERSION=$(cat .version)

machinename="$(gcc -dumpmachine)"
machinename="${VERSION}-${machinename}-tests-passed.txt"

printf "%s\n" "This is a set of PRNG tests to ensure that arbitraire ${VERSION} works" >>"${machinename}"
printf "%s\n" "properly on the following machine type:" >>"${machinename}"
printf "\n" >> "${machinename}"
uname -a >> "${machinename}"
gcc -dumpmachine >> "${machinename}"

printf "%s\n" "The tests should halt upon detecting an error and the contents" >>"${machinename}"
printf "%s\n" "of 'testing.bc' can be inspected to reveal the failing test" >>"${machinename}"
printf "\n\n" >> "${machinename}"
while [ "$COUNT" -lt 100 ]
do	COUNT="$((COUNT + 1))"
	printf "%s\n" "Test number: ${COUNT}" >>"${machinename}"
	if [ "$USE_VALGRIND" = "1" ]
	then	valgrind --leak-check=full ./tests/random-tests "$1" "$2" "$3" >log 2>log3
	elif [ "$USE_STRACE" = "1" ]
	then	strace ./tests/random-tests "$1" "$2" "$3" >log 2>log3
	elif [ "$USE_TIME" = "1" ]
	then	time ./tests/random-tests "$1" "$2" "$3" >log 2>log3
	else	./tests/random-tests "$1" "$2" "$3" >log 2>log3
	fi
	bc -l testing.bc > log2
	if diff log log2
	then	printf "%s\n" "Giant pseudo-random number test passed."
		printf "%s\n" "Giant pseudo-random number test passed." >> "${machinename}"
	else	printf "%s\n"  "Test failed! look inside of '${machinename}' to reveal the failing numbers!" 
		printf "%s\n"  "Test failed! look inside of '${machinename}' to reveal the failing numbers!" >> "${machinename}"
		exit 1
	fi
	cat testing.bc >> "${machinename}"
	[ -e log3 ] && cat log3 >> "${machinename}"
	printf "\n\n" >> "${machinename}"
	sleep 1 # we need 1 second in order to get a new random number
done


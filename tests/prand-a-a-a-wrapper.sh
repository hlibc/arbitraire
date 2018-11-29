#!/bin/sh

if [ $# -lt 1 ]
then	printf "Requires a test type, like 'div', 'add', 'sub' or 'mul'\n"
	exit 1
fi

USE_VALGRIND=0
USE_STRACE=0
if [ $# -gt 1 ]
then    if [ $2 = "valgrind" ] 
	then	USE_VALGRIND=1
	fi
	if [ $2 = "strace" ] 
	then	USE_STRACE=1
	fi
	
fi

COUNT=0

VERSION=$(cat .version)

machinename="$(gcc -dumpmachine)"
machinename="${VERSION}-${machinename}-tests-passed.txt"

rm "${machinename}"

printf "%s\n" "This is a set of PRNG tests to ensure that arbitraire ${VERSION} works" >>"${machinename}"
printf "%s\n" "properly on the following machine type:" >>"${machinename}"
printf "\n" >> "${machinename}"
uname -a >> "${machinename}"
gcc -dumpmachine >> "${machinename}"

printf "%s\n" "The tests should halt upon detecting an error and the contents" >>"${machinename}"
printf "%s\n" "of 'testing.bc' can be inspected to reveal the failing test" >>"${machinename}"
printf "\n\n" >> "${machinename}"
while [ $COUNT -lt 100 ]
do	COUNT=$((COUNT + 1))
	printf "%s\n" "Test number: ${COUNT}" >>"${machinename}"
	if [ $USE_VALGRIND = "1" ]
	then	valgrind --leak-check=full ./tests/prand-a-a-a $1 > log 2>log3
	elif [ $USE_STRACE = "1" ]
	then	strace ./tests/prand-a-a-a $1 > log
	else	./tests/prand-a-a-a $1 > log
	fi
	bc -lq testing.bc > log2
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
	sleep 1
done


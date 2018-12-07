CFLAGS += -Wall -Wextra -I./include/
SRCS = $(wildcard src/*.c)
TSRCS = $(wildcard tests/*.c)
OBJ = $(SRCS:.c=.o)
TOBJ = $(TSRCS:.c=)
LIBNAME = $(libname)
STATLIB = lib$(LIBNAME).a
DESTDIR = /
PREFIX = /lib/

LDLIBS += -L. -l$(LIBNAME)

-include config.mak

all:
	@test -e config.mak || ./configure -error
	$(MAKE) static
	$(MAKE) create_test

static: $(OBJ)
	$(AR) -cvq $(STATLIB) $(OBJ)

create_test: $(TOBJ)

clean:
	$(RM) $(OBJ) $(TOBJ) $(STATLIB) config.mak log log2 log3 testing.bc *tests-passed.txt

install:
	mkdir -p $(DESTDIR)/$(prefix)/include $(DESTDIR)/$(prefix)/lib/
	cp $(STATLIB) $(DESTDIR)/$(prefix)/lib/
	cp -r include/$(LIBNAME)/ $(DESTDIR)/$(prefix)/include


# Everything below is not an integral part of the makefile and is simply used
# for invoking arbitraire's testing mechanisms in a simple way -- `make test'
#
# Before releases, make sure to run all of the tests with valgrind mode turned
# on and run: grep 'no leaks' | wc -l on the logfile. If it outputs "1100" then
# we know that all 1100 pseudo-random tests were completed in their entirety
# and with no memory errors. Use the following command:
#	`make clean ; make release ; grep 'no leaks' *tests-passed.txt | wc -l'
#


release:
	./configure
	CFLAGS="-O3" $(MAKE) all
	echo "sqrt tests"
	./tests/random-wrapper.sh sqrt 1000 null valgrind
	echo "div tests"
	./tests/random-wrapper.sh div 10000 null valgrind
	./tests/random-wrapper.sh div 10000 agnostic valgrind
	echo "add tests"
	./tests/random-wrapper.sh add 10000 null valgrind
	./tests/random-wrapper.sh add 10000 agnostic valgrind
	echo "sub tests"
	./tests/random-wrapper.sh sub 10000 null valgrind
	./tests/random-wrapper.sh sub 10000 agnostic valgrind
	echo "mul tests"
	./tests/random-wrapper.sh mul 10000 null valgrind
	./tests/random-wrapper.sh mul 10000 agnostic valgrind
	echo "mod tests"
	./tests/random-wrapper.sh mod 10000 null valgrind
	./tests/random-wrapper.sh mod 10000 agnostic valgrind

test:
	./configure
	CFLAGS="-O3 -D_ARB_DEBUG=1" $(MAKE) all
	echo "sqrt tests"
	./tests/random-wrapper.sh sqrt 1000 null
	echo "div tests"
	./tests/random-wrapper.sh div 10000 null
	./tests/random-wrapper.sh div 10000 agnostic
	echo "add tests"
	./tests/random-wrapper.sh add 10000 null
	./tests/random-wrapper.sh add 10000 agnostic
	echo "sub tests"
	./tests/random-wrapper.sh sub 10000 null
	./tests/random-wrapper.sh sub 10000 agnostic
	echo "mul tests"
	./tests/random-wrapper.sh mul 10000 null
	./tests/random-wrapper.sh mul 10000 agnostic
	echo "mod tests"
	./tests/random-wrapper.sh mod 10000 null
	./tests/random-wrapper.sh mod 10000 agnostic



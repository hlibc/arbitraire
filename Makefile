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


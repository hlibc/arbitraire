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
	$(RM) $(OBJ) $(TOBJ) $(STATLIB) config.mak log log2 testing.bc *tests-passed.txt

install:
	mkdir -p $(DESTDIR)/$(prefix)/include $(DESTDIR)/$(prefix)/lib/
	cp $(STATLIB) $(DESTDIR)/$(prefix)/lib/
	cp -r include/$(LIBNAME)/ $(DESTDIR)/$(prefix)/include


test:
	./configure
	CFLAGS="-O3 -D_ARB_DEBUG=1" $(MAKE) all
	echo "sqrt tests"
	./tests/prand-a-a-a-wrapper.sh sqrt
	echo "div tests"
	./tests/pseudo-random-wrapper.sh div
	./tests/prand-a-a-a-wrapper.sh div
	echo "add tests"
	./tests/pseudo-random-wrapper.sh add
	./tests/prand-a-a-a-wrapper.sh add
	echo "sub tests"
	./tests/pseudo-random-wrapper.sh sub
	./tests/prand-a-a-a-wrapper.sh sub
	echo "mul tests"
	./tests/pseudo-random-wrapper.sh mul
	./tests/prand-a-a-a-wrapper.sh mul
	echo "mod tests"
	./tests/pseudo-random-wrapper.sh mod
	./tests/prand-a-a-a-wrapper.sh mod


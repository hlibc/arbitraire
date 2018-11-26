CFLAGS += -Wall -Wextra -I./include/
SRCS = $(wildcard src/*.c)
TSRCS = $(wildcard tests/*.c)
OBJ = $(SRCS:.c=.o)
TOBJ = $(TSRCS:.c=)
LIBNAME = $(libname)
STATLIB = lib$(LIBNAME).a
DESTDIR = /
PREFIX = /lib/

LDLIBS += -L. -l$(LIBNAME) -lm

-include config.mak

all:
	@test -e config.mak || { ./configure -error ; exit 1 ;};
	$(MAKE) static
	$(MAKE) create_test

static: $(OBJ)
	$(AR) -cvq $(STATLIB) $(OBJ)

create_test: $(TOBJ)

clean:
	$(RM) $(OBJ) $(TOBJ) $(STATLIB) config.mak

install:
	mkdir -p $(DESTDIR)/$(prefix)/include $(DESTDIR)/$(prefix)/lib/
	cp $(STATLIB) $(DESTDIR)/$(prefix)/lib/
	cp -r include/$(LIBNAME)/ $(DESTDIR)/$(prefix)/include


test:
	./configure
	CFLAGS="-D_ARB_DEBUG=1" $(MAKE) all
	echo "sqrt tests"
	./tests/prand-a-a-a-wrapper.sh sqrt
	echo "div tests"
	./tests/pseudo-random-wrapper.sh div
	echo "add tests"
	./tests/pseudo-random-wrapper.sh add
	echo "sub tests"
	./tests/pseudo-random-wrapper.sh sub
	echo "mul tests"
	./tests/pseudo-random-wrapper.sh mul


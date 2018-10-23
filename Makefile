CFLAGS += -Wall -Wextra -I./include/

SRCS = $(wildcard src/*.c)

TSRCS = $(wildcard tests/*.c)

OBJ = $(SRCS:.c=.o)

TEST_OBJ = $(TSRCS:.c=)

LIBNAME = arbitraire

STATLIB = lib$(LIBNAME).a

DESTDIR ?= ../

.PHONY: uninstall

PREFIX ?= /$(LIBNAME)/

LDLIBS += -l$(LIBNAME) -lm

LDFLAGS += -L.

all:

	$(MAKE) static
	$(MAKE) create_test

static: $(OBJ)

	$(MAKE) static_link

static_link:

	$(AR) -cvq $(STATLIB) $(OBJ)


create_test: $(TEST_OBJ)

cleanobj:

	$(RM) $(OBJ)

clean:
	$(MAKE) cleanobj
	$(RM) $(STATLIB)
	$(RM) $(TEST_OBJ)
	$(RM) uninstall
	$(RM) log* *-tests-* testing.bc some.bc

release:

	./.release.sh

test:
	$(MAKE) all
	./tests/prand-a-a-a-wrapper.sh sqrt

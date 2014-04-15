CWD=$(shell pwd)

AR ?= ar
CC ?= clang
PROCPS= $(CWD)/deps/procps
LIBPROC = $(PROCPS)/proc/libproc.a
LIBPROC_OBJS = $(wildcard $(PROCPS)/proc/*.o)

CFLAGS = -I$(PROCPS)
# linking didn't work, using objects for now
LDFLAGS = #-L$(LIBPROC)

build:
	$(CC) $(CFLAGS) $(LIBPROC_OBJS) main.c -o main $(LDFLAGS) 

build-pgrep:
	$(CC) $(CFLAGS) $(LIBPROC_OBJS) main-pgrep.c -o main-pgrep $(LDFLAGS) 

run: build
	./main

run-pgrep: build-pgrep
	./main-pgrep sh

$(LIBPROC):
	cd $(PROCPS) && $(MAKE) proc/libproc.a
libproc: $(LIBPROC)

clean:
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
	rm -f main main-pgrep

cleanall: clean
	cd $(CWD)/deps/procps && $(MAKE) clean 

.PHONY: libproc clean cleanall

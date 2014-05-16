include v8.mk

ROOT	  = $(dir $(lastword $(MAKEFILE_LIST)))

AR ?= ar
CC ?= gcc
CXX = g++
LINK = g++
PROCPS= $(ROOT)deps/procps/
LIBPROC = $(PROCPS)proc/libproc.a

CFLAGS = -I$(PROCPS) -I$(V8)include/
LDFLAGS =                                                        \
  $(LIBPROC)                                                     \
	-Wl,--start-group                                              \
			$(V8)/out/$(V8_ARCH)/obj.target/tools/gyp/libv8_base.a     \
			$(V8)/out/$(V8_ARCH)/obj.target/tools/gyp/libv8_snapshot.a \
	-Wl,--end-group                                                \
	-lpthread                                                      \
	-lrt 

build: $(LIBPROC) $(V8LIB)
	$(CXX) $(CFLAGS) read_file.cc main.cc proc.cc procjs.cc -o main $(LDFLAGS) 

build-print: $(LIBPROC)
	$(CC) $(CFLAGS) print_proctab.c -o print_proctab $(LDFLAGS) 

run: build
	./main

time: build
	time ./main > /dev/null

run-print: build-print
	./print_proctab

$(LIBPROC):
	cd $(PROCPS) && $(MAKE) proc/libproc.a
libproc: $(LIBPROC)

clean:
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
	rm -f main print_proctab
	rm -f *.o

cleanall: clean
	cd $(ROOT)deps/procps && $(MAKE) clean 

.PHONY: libproc clean cleanall run time build

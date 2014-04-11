CWD=$(shell pwd)

AR ?= ar
CC ?= clang

LIBPROC_A = $(CWD)/deps/procps/proc/libproc.a

$(LIBPROC_A):
	cd $(CWD)/deps/procps && $(MAKE) proc/libproc.a
libproc: $(LIBPROC_A)

clean:
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

cleanall: clean
	cd $(CWD)/deps/procps && $(MAKE) clean 

.PHONY: libproc clean cleanall

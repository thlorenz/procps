CXX         = g++
LINK        = g++
GYP_DEFINES = 

V8      = $(ROOT)deps/v8/
V8_REPO = $(V8).gitignore
V8_LIB  = $(V8)out/native/obj.target/tools/gyp/libv8_{base.x64,snapshot}.a
V8_GYP  = $(V8)build/gyp/gyp
V8_ARCH ?= native
V8_VERSION ?= 3.14.5

v8: $(V8_LIB)
	
$(V8_LIB): $(V8_REPO) $(V8_GYP)
	cd $(V8)  && \
	CXX=$(CXX) LINK=$(LINK) GYP_DEFINES=$(GYP_DEFINES) werror=no $(MAKE) $(V8_ARCH) -j4 console=readline

$(V8_REPO):
	cd $(ROOT)deps && \
	curl -L https://github.com/v8/v8/archive/$(V8_VERSION).tar.gz | tar xvzf - && \
	mv v8-$(V8_VERSION) v8

# was unable to checkout gyp via svn so did this instead
# cd deps/v8 && mkdir build && git clone https://github.com/svn2github/gyp build/gyp
# cd build/gyp && git reset f7bc250 --hard 
# f7bc250 being the same revision (1501) that the v8 build is trying to checkout from svn
$(V8_GYP):
	cd $(V8) && CXX=$(CXX) LINK=$(LINK) GYP_DEFINES=$(GYP_DEFINES) $(MAKE) dependencies

.PHONY: v8

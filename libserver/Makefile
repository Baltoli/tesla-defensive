TESLA = $(abspath $(TESLA_ROOT)/bin/tesla)
TESLA_INCLUDE = $(TESLA_ROOT)/include
TESLA_LINK = $(TESLA_ROOT)/lib

INSTALL_PREFIX = $(HOME)/libserver_install/

CC = clang34
LLVM_LINK = llvm-link34

CFLAGS += -g -std=c11 -Wall -pedantic -Wno-gnu-empty-struct

LIB_NAME = server
SRCS = server.c
INCLUDES = server.h

.DEFAULT_GOAL := all
all: lib$(LIB_NAME).m.bc

%.manifest : %.m.bc
	tesla extract $? > $@

%.static.manifest : %.m.bc
	$(eval TMP := $(shell mktemp))
	tesla extract $? > $(TMP)
	$(TESLA) static $(TMP) $? -modelcheck -bound=1000 -o $@

%.tesla : %.c
	$(TESLA) analyse $(abspath $<) -o $@ -- -DTESLA $(CFLAGS) -I$(TESLA_INCLUDE)

tesla.manifest : $(SRCS:.c=.tesla)
	$(TESLA) cat $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -I$(TESLA_INCLUDE) $(abspath $<) -c -o $@

%.bc : %.c
	$(CC) $(CFLAGS) -emit-llvm -I$(TESLA_INCLUDE) -DTESLA $(abspath $<) -c -o $@

lib$(LIB_NAME).bc : $(SRCS:.c=.bc)
	$(LLVM_LINK) $^ -o $@

lib$(LIB_NAME).m.bc : lib$(LIB_NAME).bc tesla.manifest
	tesla archive $^ -o $@

install : lib$(LIB_NAME).m.bc $(INCLUDES)
	mkdir -p $(INSTALL_PREFIX)/tlib
	mkdir -p $(INSTALL_PREFIX)/include
	cp $(INCLUDES) $(INSTALL_PREFIX)/include
	cp $< $(INSTALL_PREFIX)/tlib

clean:
	rm -rf *.bc *.tesla *.manifest

TESLA = $(abspath $(TESLA_ROOT)/bin/tesla)
TESLA_INCLUDE = $(TESLA_ROOT)/include
TESLA_LINK = $(TESLA_ROOT)/lib

CC = clang34
LLVM_LINK = llvm-link34

CFLAGS += -g -std=c11 -Wall -pedantic -Wno-gnu-empty-struct

%.manifest : %.m.bc
	tesla extract $? > $@

%.static.manifest : %.m.bc
	$(eval TMP := $(shell mktemp))
	tesla extract $? > $(TMP)
	$(TESLA) static $(TMP) $? -modelcheck -bound=1000 -o $@

clean::
	rm -rf *.tesla *.manifest *.bc *.o

include ../Common.mk

LIB_NAME = kv
SRCS = lib.c \
       server.c \
       main.c

.DEFAULT_GOAL := all

all: lib$(LIB_NAME).m.bc

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

install : lib$(LIB_NAME).m.bc
	cp lib$(LIB_NAME).m.bc ../app

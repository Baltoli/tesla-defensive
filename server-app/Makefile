.DEFAULT_GOAL := all

TESLA = $(abspath $(TESLA_ROOT)/bin/tesla)
TESLA_INCLUDE = $(TESLA_ROOT)/include
TESLA_LINK = $(TESLA_ROOT)/lib

CC = clang34
LLVM_LINK = llvm-link34

LIB_INSTALL_PREFIX = $(HOME)/libserver_install

LIB_INCLUDE = -I$(LIB_INSTALL_PREFIX)/include
LDFLAGS = -Wl,-rpath,$(TESLA_LINK) -L$(TESLA_LINK) -ltesla -lpthread

SRCS = main.c
APP_NAME = server
LIB_ID = server
LIB_NAME = lib$(LIB_ID).m.bc

all : $(APP_NAME) $(APP_NAME).static

%.m.bc:
	cp $(LIB_INSTALL_PREFIX)/tlib/$@ .

%.bc : %.c
	$(CC) $(CFLAGS) $(LIB_INCLUDE) -emit-llvm $(abspath $<) -c -o $@

$(APP_NAME).instr.bc : $(LIB_NAME) $(SRCS:.c=.bc)
	$(eval ITMP := $(shell mktemp))
	$(LLVM_LINK) $^ -o $(APP_NAME).bc
	$(TESLA) extract $< > $(ITMP)
	$(TESLA) instrument -tesla-manifest $(ITMP) $(APP_NAME).bc -o $@

$(APP_NAME).static.instr.bc : $(LIB_NAME) $(SRCS:.c=.bc)
	$(eval STMP := $(shell mktemp))
	$(eval STMP2 := $(shell mktemp))
	$(LLVM_LINK) $^ -o $(APP_NAME).bc
	$(TESLA) extract $< > $(STMP)
	$(TESLA) static $(STMP) $(APP_NAME).bc -modelcheck -bound=1000 -o $(STMP2)
	$(TESLA) instrument -tesla-manifest $(STMP2) $(APP_NAME).bc -o $@

$(APP_NAME) : $(APP_NAME).instr.bc
	$(CC) $(LDFLAGS) $(CFLAGS) $? -o $@

$(APP_NAME).static : $(APP_NAME).static.instr.bc
	$(CC) $(LDFLAGS) $(CFLAGS) $? -o $@

clean:
	rm -rf *.bc $(APP_NAME) $(APP_NAME).static

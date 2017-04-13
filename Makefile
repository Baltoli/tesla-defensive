TESLA = $(TESLA_ROOT)/bin/tesla
TESLA_INCLUDE = $(TESLA_ROOT)/include
TESLA_LINK = $(TESLA_ROOT)/lib
CC = clang34
LLVM_LINK = llvm-link34

APP_NAME = app
SRCS = main.c \
       lib.c \
       server.c \
       plugin.c

CFLAGS += -g -std=c11 -Wall -pedantic -Wno-gnu-empty-struct
CFLAGS += -pthread

all: $(APP_NAME) $(APP_NAME)_tesla $(APP_NAME)_static

%.tesla : %.c
	$(TESLA) analyse $(abspath $<) -o $@ -- -DTESLA $(CFLAGS) -I$(TESLA_INCLUDE)

tesla.manifest : $(SRCS:.c=.tesla)
	$(TESLA) cat $^ -o $@

tesla_static.manifest: tesla.manifest app.bc
	$(TESLA) static $^ -modelcheck -bound=1000 -o $@

%.o : %.c
	$(CC) $(CFLAGS) -I$(TESLA_INCLUDE) $(abspath $<) -c -o $@

%.bc : %.c
	$(CC) $(CFLAGS) -emit-llvm -I$(TESLA_INCLUDE) -DTESLA $(abspath $<) -c -o $@

app.bc : $(SRCS:.c=.bc)
	$(LLVM_LINK) $^ -o $@

app.instr.bc : tesla.manifest app.bc
	$(TESLA) instrument -tesla-manifest $^ -o $@

app_static.instr.bc : tesla_static.manifest app.bc
	$(TESLA) instrument -tesla-manifest $^ -o $@

$(APP_NAME)_tesla : app.instr.bc
	$(CC) $(APP_CFLAGS) $(CFLAGS) -L$(TESLA_LINK) -Wl,-rpath,$(TESLA_LINK) -ltesla $(abspath $<) -o $@

$(APP_NAME)_static : app_static.instr.bc
	$(CC) $(APP_CFLAGS) $(CFLAGS) -L$(TESLA_LINK) -Wl,-rpath,$(TESLA_LINK) -ltesla $(abspath $<) -o $@

$(APP_NAME) : $(SRCS:.c=.o)
	$(CC) $(APP_CFLAGS) $(CFLAGS) $(abspath $^) -o $@

clean:
	rm -rf $(APP_NAME) $(APP_NAME)_tesla $(APP_NAME)_static 
	rm -rf *.tesla *.manifest *.bc *.o

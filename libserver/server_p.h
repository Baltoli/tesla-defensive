#ifndef SERVER_P_H
#define SERVER_P_H

#include "server.h"

struct server_thread_args {
  int fd;
};

void *server_thread(void *args);

#endif

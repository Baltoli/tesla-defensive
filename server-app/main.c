#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <server/server.h>

void handle_data(size_t len, uint8_t *data) 
{
  data[len-1] = 0;
  printf("Received: %s\n", data);
}

int main(int argc, char **argv)
{
  int fd = server_socket(4567);
  server_loop(fd);
  return 0;
}

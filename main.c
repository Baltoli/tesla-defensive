#include <stdlib.h>

#include <tesla-macros.h>

#include "lib.h"
#include "server.h"

int main(int argc, char **argv)
{
  if(argc < 2) {
    fprintf(stderr, "Not enough args\n");
    return 1;
  }

  int sockfd = server_socket(atoi(argv[1]));
  if(sockfd < 0) {
    return 1;
  }

  server_loop(sockfd);

  return 1;
}

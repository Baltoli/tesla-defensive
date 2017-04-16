#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include <netinet/in.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <tesla-macros.h>

#include "server_p.h"

int main(int argc, char **argv);

int server_socket(int port)
{
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("Opening socket");
    return -1;
  }

  if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0) {
    perror("sockopt");
    return -1;
  }

  if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &(int){ 1 }, sizeof(int)) < 0) {
    perror("sockopt");
    return -1;
  }

  struct sockaddr_in serv_addr = { 0 };
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);
  serv_addr.sin_addr.s_addr = INADDR_ANY;

  if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("Binding");
    return -1;
  }

  return sockfd;
}

void server_loop(int sockfd)
{
#ifdef TESLA
  TESLA_WITHIN(main, previously(returnfrom(server_socket)));
#endif

  listen(sockfd, 5);

  while(true) {
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);

    int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if(newsockfd < 0) {
      perror("Accepting");
      return;
    }

    pthread_t thread;
    struct server_thread_args args = { .fd = newsockfd };

    pthread_create(&thread, NULL, server_thread, &args);
  }
}

void *server_thread(void *a)
{
  struct server_thread_args *args = (struct server_thread_args *)a;
  (void)args;

  const size_t read_size = 10;

  uint8_t in_buf[read_size];
  bzero(in_buf, read_size);

  uint8_t *msg_buf = NULL;
  size_t msg_len = 0;

  int n_read;
  while(true) {
    n_read = read(args->fd, in_buf, read_size);
    if(n_read <= 0) { break; }

    msg_buf = realloc(msg_buf, msg_len + read_size);
    memcpy(msg_buf + msg_len, in_buf, read_size);

    if(n_read == read_size) {
      msg_len += read_size;
    } else {
      msg_len += (size_t)n_read;

      if(handle_data) {
        handle_data(msg_len, msg_buf);
      }
      
      free(msg_buf);

      msg_buf = NULL;
      msg_len = 0;
    }
  }

  pthread_exit(0);
}

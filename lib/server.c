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

#include "plugin-interface.h"
#include "server.h"

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

  char buffer[1024] = { 0 };
  while(true) {
    int n = read(args->fd, buffer, 1024);
    char msg[1024] = { 0 };
    int off = 0;

    for(int i = 0; i < n; i++) {
      if(buffer[i] == '\n') {
        msg[i+1-off] = '\0';
        handle_message(args->fd, msg);
        memset(msg, 0, 1024);
        off = i+1;
      } else {
        msg[i-off] = buffer[i];
      }
    }
  }
}

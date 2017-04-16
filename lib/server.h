#ifndef SERVER_H
#define SERVER_H

struct server_thread_args {
  int fd;
};

int server_socket(int port);
void server_loop(int sockfd);
void *server_thread(void *args);

#endif

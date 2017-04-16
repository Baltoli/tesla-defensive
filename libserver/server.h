#ifndef SERVER_H
#define SERVER_H

int server_socket(int port);
void server_loop(int sockfd);

void receive_data(size_t len, uint8_t *data) __attribute__((weak));

#endif

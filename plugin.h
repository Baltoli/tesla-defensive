#ifndef PLUGIN_H
#define PLUGIN_H

enum command {
  put, get,
  num_commands
};

struct put_args {
  size_t key_begin;
  size_t key_len;
  size_t val_begin;
  size_t val_len;
};

struct get_args {
  size_t key_begin;
  size_t key_len;
};

enum command parse_command(char *buf);
struct put_args parse_put(char *buf);
struct get_args parse_get(char *buf);

void handle_put(char *message, struct put_args args);
void handle_get(char *message, struct get_args args);

#endif

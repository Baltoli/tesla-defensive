#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "plugin-interface.h"
#include "plugin.h"

enum command parse_command(char *buf)
{
  if(strncmp(buf, "PUT:", 4) == 0) { return put; }
  if(strncmp(buf, "GET:", 4) == 0) { return get; }
  return -1;
}

struct put_args parse_put(char *buf)
{
  struct put_args ret = {
    .key_begin = 4, .key_len = 0,
    .val_begin = 4, .val_len = 0
  };

  char *p = buf + 4;
  while(*p != '=') {
    ret.key_len++;
    ret.val_begin++;

    if(*p == '\0') {
      struct put_args err = { 0 };
      return err;
    }

    p++;
  }

  ret.val_begin++; p++;
  while(*++p != '\0') {
    ret.val_len++;
  }

  return ret;
}

struct get_args parse_get(char *buf)
{
  struct get_args ret = {
    .key_begin = 4, .key_len = strlen(buf) - 5
  };
  return ret;
}

void handle_put(char *message, struct put_args args)
{
  char *key = malloc(args.key_len + 1);
  char *val = malloc(args.val_len + 1);

  strncpy(key, message+args.key_begin, args.key_len);
  key[args.key_len] = '\0';

  strncpy(val, message+args.val_begin, args.val_len);
  val[args.val_len] = '\0';

  printf("Setting %s to %s\n", key, val);
}

void handle_get(char *message, struct get_args args)
{
  char *key = malloc(args.key_len + 1);
  strncpy(key, message+args.key_begin, args.key_len);
  key[args.key_len] = '\0';

  printf("Getting value of %s\n", key);
}

void handle_message(char *message)
{
  enum command c = parse_command(message);
  switch(c) {
    case put:
      handle_put(message, parse_put(message));
      break;
    case get:
      handle_get(message, parse_get(message));
      break;
    default:
      printf("Error\n");
      break;
  }
}

#ifndef LIB_H
#define LIB_H

#include <tesla-macros.h>

#include <stdio.h>

int main(int argc, char **argv);

FILE *open_log_file(void);
int close_log_file(void);

struct stats {
  unsigned int hits;
};

void acquire_stats_lock(void);
void release_stats_lock(void);

#endif

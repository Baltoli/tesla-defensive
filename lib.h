#ifndef LIB_H
#define LIB_H

#include <stdio.h>
#include <pthread.h>

#include <tesla-macros.h>

#include "plugin-interface.h"

struct stats {
  unsigned n_reqs;
  unsigned n_errs;
};

void lock_stats();
void unlock_stats();
void record_req();
void record_err();
void dump_stats();

void lock_data_file();
void unlock_data_file();
FILE *open_data_file();
void close_data_file();

void terminate_connection();

#endif

#include "lib.h"
#include "server.h"

#include <tesla-macros.h>

static struct stats stats = {
  .n_reqs = 0,
  .n_errs = 0
};

static pthread_mutex_t stats_lock = PTHREAD_MUTEX_INITIALIZER;

static FILE *data_file = NULL;
static pthread_mutex_t data_file_lock = PTHREAD_MUTEX_INITIALIZER;

void lock_stats()
{
#ifdef TESLA
  TESLA_WITHIN(handle_message, ATLEAST(1, TSEQUENCE(
    TESLA_ASSERTION_SITE,
    returnfrom(unlock_stats)
  )));
#endif
  pthread_mutex_lock(&stats_lock);
}

void unlock_stats()
{
#ifdef TESLA
  TESLA_WITHIN(handle_message, ATLEAST(1, TSEQUENCE(
    returnfrom(lock_stats),
    TESLA_ASSERTION_SITE
  )));
#endif
  pthread_mutex_unlock(&stats_lock);
}

void record_req()
{
#ifdef TESLA
  TESLA_WITHIN(handle_message, ATLEAST(1, TSEQUENCE(
    returnfrom(lock_stats),
    TESLA_ASSERTION_SITE
  )));
#endif
  stats.n_reqs++;
}

void record_err()
{
#ifdef TESLA
  TESLA_WITHIN(handle_message, ATLEAST(1, TSEQUENCE(
    returnfrom(lock_stats),
    TESLA_ASSERTION_SITE
  )));
#endif
  stats.n_errs++;
}

void dump_stats()
{
#ifdef TESLA
  TESLA_WITHIN(handle_message, ATLEAST(1, TSEQUENCE(
    returnfrom(lock_stats),
    TESLA_ASSERTION_SITE
  )));
#endif
  printf("Server statistics:\n\t%u requests\n\t%u errors\n",
          stats.n_reqs, stats.n_errs);
}

void lock_data_file()
{
#ifdef TESLA
  TESLA_WITHIN(handle_message, ATLEAST(1, eventually(
    returnfrom(unlock_data_file))));
#endif
  pthread_mutex_lock(&data_file_lock);
}

void unlock_data_file()
{
#ifdef TESLA
  TESLA_WITHIN(handle_message, ATLEAST(1, previously(
    returnfrom(lock_data_file))));
#endif
  pthread_mutex_unlock(&data_file_lock);
}

FILE *open_data_file()
{
#ifdef TESLA
  TESLA_WITHIN(handle_message, ATLEAST(1, TSEQUENCE(
    returnfrom(lock_data_file),
    TESLA_ASSERTION_SITE,
    returnfrom(close_data_file),
    returnfrom(unlock_data_file)
  )));
#endif
  data_file = fopen("data.txt", "a+");
  return data_file;
}

void close_data_file()
{
#ifdef TESLA
  TESLA_WITHIN(handle_message, ATLEAST(1, TSEQUENCE(
    returnfrom(lock_data_file),
    returnfrom(open_data_file),
    TESLA_ASSERTION_SITE,
    returnfrom(unlock_data_file)
  )));
#endif
  fclose(data_file);
  data_file = NULL;
}

void terminate_connection()
{
  pthread_exit(0);
}

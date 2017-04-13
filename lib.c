#include "lib.h"
#include "server.h"

static struct stats stats = {
  .n_reqs = 0,
  .n_errs = 0
};
static pthread_mutex_t stats_lock = PTHREAD_MUTEX_INITIALIZER;

void lock_stats()
{
#ifdef TESLA
  TESLA_WITHIN(handle_message, TSEQUENCE(
    TESLA_ASSERTION_SITE,
    returnfrom(unlock_stats)
  ));
#endif
  pthread_mutex_lock(&stats_lock);
}

void unlock_stats()
{
#ifdef TESLA
  TESLA_WITHIN(handle_message, TSEQUENCE(
    returnfrom(lock_stats),
    TESLA_ASSERTION_SITE
  ));
#endif
  pthread_mutex_unlock(&stats_lock);
}

void record_req()
{
#ifdef TESLA
  TESLA_WITHIN(handle_message, TSEQUENCE(
    returnfrom(lock_stats),
    TESLA_ASSERTION_SITE,
    returnfrom(unlock_stats)
  ));
#endif
  stats.n_reqs++;
}

void record_err()
{
#ifdef TESLA
  TESLA_WITHIN(handle_message, TSEQUENCE(
    returnfrom(lock_stats),
    TESLA_ASSERTION_SITE,
    returnfrom(unlock_stats)
  ));
#endif
  stats.n_errs++;
}

void dump_stats()
{
#ifdef TESLA
  TESLA_WITHIN(handle_message, TSEQUENCE(
    returnfrom(lock_stats),
    TESLA_ASSERTION_SITE,
    returnfrom(unlock_stats)
  ));
#endif
  printf("Server statistics:\n\t%u requests\n\t%u errors\n",
          stats.n_reqs, stats.n_errs);
}

void terminate_connection()
{
  pthread_exit(0);
}

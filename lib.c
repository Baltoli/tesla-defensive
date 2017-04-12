#include "lib.h"

static FILE *log_file = NULL;
/*static struct stats stats = {
  .hits = 0
};*/

FILE *open_log_file(void)
{
#ifdef TESLA
  TESLA_WITHIN(main, ATLEAST(1, TSEQUENCE(
    TESLA_ASSERTION_SITE,
    call(close_log_file)
  )));
#endif

  if(!log_file) {
    log_file = fopen("log.txt", "a");
  }

  return log_file;
}

int close_log_file(void)
{
#ifdef TESLA
  TESLA_WITHIN(main, ATLEAST(1, TSEQUENCE(
    call(open_log_file),
    TESLA_ASSERTION_SITE
  )));
#endif

  if(log_file) {
    return fclose(log_file);
  }

  return 0;
}

void acquire_stats_lock(void)
{
#ifdef TESLA
  TESLA_WITHIN(main, ATLEAST(1, TSEQUENCE(
    TESLA_ASSERTION_SITE,
    call(release_stats_lock)
  )));
#endif
}

void release_stats_lock(void)
{
#ifdef TESLA
  TESLA_WITHIN(main, ATLEAST(1, TSEQUENCE(
    call(acquire_stats_lock),
    TESLA_ASSERTION_SITE
  )));
#endif
}

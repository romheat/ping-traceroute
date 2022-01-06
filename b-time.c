#include <time.h>
#include <stdlib.h>
#include "b-time.h"

/* time */

struct timespec time_diff(struct timespec start, struct timespec end)
{
  struct timespec elapsed;
  elapsed.tv_nsec = end.tv_nsec - start.tv_nsec;
  elapsed.tv_sec = end.tv_sec - start.tv_sec;
  if (elapsed.tv_sec > 0 && elapsed.tv_nsec < 0)
  {
    elapsed.tv_nsec += NANO_TO_SEC;
    elapsed.tv_sec--;
  }
  else if (elapsed.tv_sec < 0 && elapsed.tv_nsec > 0)
  {
    elapsed.tv_nsec -= NANO_TO_SEC;
    elapsed.tv_sec++;
  }
  return elapsed;
}

double time_diff_sec(struct timespec start, struct timespec end)
{
  struct timespec elapsed = time_diff(start, end);
  return (double)elapsed.tv_sec + (elapsed.tv_nsec / NANO_TO_SEC);
}

double time_diff_ms(struct timespec start, struct timespec end)
{
  struct timespec elapsed = time_diff(start, end);
  return (double)(elapsed.tv_sec / 1000) + (elapsed.tv_nsec / NANO_TO_MS);
}

double time_diff_ns(struct timespec start, struct timespec end)
{
  struct timespec elapsed = time_diff(start, end);
  return (double)(elapsed.tv_sec / NANO_TO_SEC) + elapsed.tv_nsec;
}

struct time_stats *init_time_stats()
{
  struct time_stats *stats = malloc(sizeof(struct time_stats));
  stats->total = 0;
  stats->min = 10000;
  stats->max = 0;
  stats->n = 0;
  return stats;
}

void update_time_stats(double elapsed, struct time_stats *stats)
{
  if (elapsed > stats->max)
    stats->max = elapsed;
  if (stats->min > elapsed)
    stats->min = elapsed;
  stats->total += elapsed;
  stats->n++;
  stats->last_value = elapsed;
}
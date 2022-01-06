#ifndef B_TOOLS_H
#define B_TOOLS_H
#include <time.h>

/* macros for elapsed time */

#define NANO_TO_SEC 1000000000.0
#define NANO_TO_MS 1000000.0


#define Elapsed() struct timespec start, end;
#define ElapsedStart() clock_gettime(CLOCK_REALTIME, &start);
#define ElapsedEnd() clock_gettime(CLOCK_REALTIME, &end);

#define ELAPSED_MS time_diff_ms(start, end)
#define ELAPSED_SEC time_diff_sec(start, end)
#define ELAPSED_NS time_diff_ns(start, end)

#define MB_SIZE (1024 * 1024)
#define SPEED_MBS(size) (((size) / ELAPSED_SEC) / MB_SIZE)

struct time_stats
{
  unsigned int n;
  double min;
  double max;
  double total;
  double last_value;
};

struct time_stats *init_time_stats();
void update_time_stats(double elapsed, struct time_stats *stats);

/* time */
long time_nanos();

double time_diff_sec(struct timespec start, struct timespec end);
double time_diff_ms(struct timespec start, struct timespec end);
double time_diff_ns(struct timespec start, struct timespec end);

#endif
/*

  Hello Mr.Tsoding! Really appreciate your work. Thank you!

  I wanted to share with you some cool stuff about calculation of mean
  and stdev. Have no idea how to do it other way, feel free to close
  this PR :)

  A few days before I watched your recent videos I was tackling a
  similar problem in my app. It generates thousands of events per
  second with metadata about running computations — timing, iteration,
  etc. I collect that data and show averages and standard deviations
  for a bunch of series, so I need to recompute them every frame. The
  brute-force way required two full passes over each list: one for the
  mean and another for the stdev.

  I found a tiny, handy structure that lets me do this — it easily
  allows me recalculate the stdev for datasets containing hundreds of
  millions of values at 180 frames per second.

  The idea is rather simple.
 */

#include <stdio.h>
#include <math.h>


typedef struct Stats {
  /* Let's cache the current count and the sum of elements. Those two
     values are trivial to update whenever we add a new value. */

  float n; // number of items is in the data set

  /* 1. Average

  avg = ∑(xi) / N;

  The core idea is to eliminate the need to traverse elements each
  time a new value is added to the set. If we keep the sum and count
  cached, the average is immediately available as sum / N.
  */
  float sum;

  /* 2. standard deviation
              ---------------
             /  ∑(a - xi)²
  stdev =   /   -----------
           V         N

  What about the stdev — can we avoid that second pass? Let’s look at
  the numerator inside the square root:

     ∑(a - xi)² ; where a = average, xi =i-th element of the data set

  Let's expand the square:
  1. (a - b)² = a² - 2ab + b²

     ∑(a - xi)² = (a - x1)² + (a - x2)² + .. (a - xn)² =
     = a² - 2a*x1 + x1² + a² - 2a*x2 + x² + .. + a² - 2a*xn + xn² =

     ... let's group addends

     = (a² + a² .. + a²) - 2a*x1 - 2a*x2 - .. - 2a*xn + (x1² + x2².. xn²) =
       ~~~~~~~~~~~~~~~~~   ~~~~~~~~~~~~~~~~~~~~~~~~~~   ~~~~~~~~~~~~~~~~~
            n times          common multiplier 2a        sum of squares

     = n*a² - 2a*(x1 + x2 + .. xn) - ∑xi²
                 ~~~~~~~~~~~~~~~~~
                   sum of values

     = n*a² - 2a*(∑xi) - ∑xi².


  Nice — we already have most values cached from the previous step: N,
  the average, and the sum. We can also keep a running sum of squares
  and update it on the fly. That’s all — no need to keep the full
  list: just track count, sum, and sum of squares.
  */
  float sq_sum;

} Stats;

// Now we able to account new values on the fly...
void stats_account(Stats *stats, float val)
{
  stats->n += 1;
  stats->sum += val;
  stats->sq_sum += val*val;
}

typedef struct Sample {
  float avg;
  float stdev;
} Sample;

// ...as well as calculate average and stdev.
Sample stats_sample(Stats s)
{
  float avg = s.sum / s.n ;
  float stdev = sqrt((avg*avg*s.n - 2*s.sum*avg + s.sq_sum)/s.n);
  return (Sample){ .avg = avg, .stdev = stdev };
}

// The neat part: we also able to merge two Stats! A piece of cake:
void stats_merge_with(Stats *s_dest, Stats s_src)
{
  s_dest->n += s_src.n;
  s_dest->sum += s_src.sum;
  s_dest->sq_sum += s_src.sq_sum;
}

// If you want, windowing is also possible, since we can also easily
// remove elements from accumulators we use:
void stats_drop_value(Stats *s_desc, float val)
{
  s_desc->n -= 1;
  s_desc->sum -= val;
  s_desc->sq_sum -= val*val;
}

// It's also possible two exclude one accumulated data set from
// another!
void stats_exclude(Stats *s_dest, Stats s)
{
  s_dest->n -= s.n;
  s_dest->sum -= s.sum;
  s_dest->sq_sum -= s.sq_sum;
}

/* If you plan only to collect data without the need to remove
   something from the data sets, you also can easily have at least
   following two immediate aggregates cached: min and max. */

void dump_stats(Stats s)
{
  /* printf("--------------------------------------------------\n"); */
  /* printf("N=%f; ∑=%f; ∑²=%f\n", s.n, s.sum, s.sq_sum); */
  Sample sample = stats_sample(s);
  printf("μ=%f, σ=%f\n", sample.avg, sample.stdev);
  /* printf("--------------------------------------------------\n"); */
}

int main()
{
  printf("Basic continuous value accumulation:\n");
  Stats s = {0};
  stats_account(&s, 10);
  dump_stats(s);

  stats_account(&s, 20);
  dump_stats(s);

  stats_account(&s, 30);
  dump_stats(s);

  stats_account(&s, 69);
  dump_stats(s);

  stats_account(&s, 13);
  dump_stats(s);

  printf("\n");

  printf("Adding values equal to the mean to a dataset does not change\n");
  printf("the mean, but it reduces the variance (and therefore the\n");
  printf("standard deviation).:\n");
  stats_account(&s, 20);
  dump_stats(s);

  stats_account(&s, 20);
  dump_stats(s);

  stats_account(&s, 20);
  dump_stats(s);

  stats_account(&s, 20);
  dump_stats(s);

  stats_account(&s, 20);
  dump_stats(s);

  stats_account(&s, 20);
  dump_stats(s);
  printf("\n");

  printf("Adding values far from the current mean can dramatically\n");
  printf("increase the standard deviation:\n");
  stats_account(&s, 50);
  dump_stats(s);

  stats_account(&s, 80);
  dump_stats(s);

  stats_account(&s, -23);
  dump_stats(s);

  stats_account(&s, -100);
  dump_stats(s);
  printf("\n");

  printf("Merging stats:\n");
  Stats s1 = {0};
  stats_account(&s1, 10);

  Stats s2 = {0};
  stats_account(&s1, 20);

  stats_merge_with(&s1, s2);
  dump_stats(s1);
  printf("\n");

  printf("Removals:\n");
  Stats s3 = {0};
  stats_account(&s3, 10);
  dump_stats(s3);

  stats_account(&s3, 15);
  dump_stats(s3);

  stats_account(&s3, 13);
  dump_stats(s3);

  stats_drop_value(&s3, 13);
  dump_stats(s3);

  stats_drop_value(&s3, 15);
  dump_stats(s3);

  stats_merge_with(&s3, s1);
  dump_stats(s3);

  stats_exclude(&s3, s1);
  dump_stats(s3);
}

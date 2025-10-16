// Utilities used by both loopers and sleepers
#ifndef WORKER_UTILS_H_
#define WORKER_UTILS_H_

#ifdef _WIN32
#include <windows.h>
#define sleep_ms Sleep
#define get_seed GetCurrentProcessId
#else
#include <unistd.h>
#define sleep_ms(ms) usleep(ms*1000);
#define get_seed getpid
#endif

#define rand_float() ((float)rand()/RAND_MAX)
#define rand_double() ((double)rand()/RAND_MAX)

#endif // WORKER_UTILS_H_

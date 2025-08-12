#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#define trace_temp_alloc(requested_size) \
    (temp_alloc(requested_size), \
     printf("temp_alloc(%3zu) → temp_size == %3zu\n", requested_size, nob_temp_size))

int main(void)
{
    static size_t allocs[] = { 0, 2, 3, 5, 7, 11, 13, 11, 7, 5, 3, 2 };

    for (size_t i = 0; i < ARRAY_LEN(allocs); ++i) {
        trace_temp_alloc(allocs[i]);
    }

    return 0;
}

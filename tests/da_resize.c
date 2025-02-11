#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_DA_INIT_CAP 4
#include "nob.h"

typedef struct {
    int *items;
    size_t count;
    size_t capacity;
} Numbers;

int main(void)
{
    Numbers xs = {0};
    da_append(&xs, 69);
    da_append(&xs, 420);
    da_append(&xs, 1337);
    nob_log(INFO, "count = %zu, capacity = %zu", xs.count, xs.capacity);
    nob_da_resize(&xs, 1);
    nob_log(INFO, "count = %zu, capacity = %zu", xs.count, xs.capacity);
    nob_da_resize(&xs, 10);
    nob_log(INFO, "count = %zu, capacity = %zu", xs.count, xs.capacity);
    return 0;
}

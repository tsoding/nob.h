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
    for (int x = 0; x < 10; ++x) {
        da_append(&xs, x);
        nob_log(INFO, "count = %zu, capacity = %zu", xs.count, xs.capacity);
    }
    return 0;
}

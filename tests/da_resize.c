#define NOB_DA_INIT_CAP 4
#include "shared.h"

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
    printf("count = %zu, capacity = %zu\n", xs.count, xs.capacity);
    nob_da_resize(&xs, 1);
    printf("count = %zu, capacity = %zu\n", xs.count, xs.capacity);
    nob_da_resize(&xs, 10);
    printf("count = %zu, capacity = %zu\n", xs.count, xs.capacity);
    return 0;
}

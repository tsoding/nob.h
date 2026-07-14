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
    for (int x = 0; x < 10; ++x) {
        da_append(&xs, x);
        printf("count = %zu, capacity = %zu\n", xs.count, xs.capacity);
    }
    return 0;
}

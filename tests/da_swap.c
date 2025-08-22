#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

typedef struct {
    int *items;
    size_t count;
    size_t capacity;
} Numbers;

int main(void)
{
    nob_log(INFO, "da_swap:");
    Numbers xs = {0};
    for (int i = 1; i <= 10; ++i) da_append(&xs, i);
    for (int i = 0; i < 5; ++i) da_swap(&xs, i, xs.count - i - 1);
    for (int i = 0; i < 10; ++i) nob_log(INFO, "%d", xs.items[i]);
    return 0;
}

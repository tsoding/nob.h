#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

int main()
{
    typedef struct {
        int *items;
        size_t count;
        size_t capacity;
    } Numbers;
    Numbers xs = {0};
    da_append(&xs, 69);
    da_append(&xs, 420);
    da_append(&xs, 1337);
    da_foreach(int, x, &xs) {
        // `x` here is a pointer to the current element. You can get its index by taking a difference
        // between `x` and the start of the array which is `x.items`.
        size_t index = x - xs.items;
        nob_log(INFO, "%zu: %d", index, *x);
    }
    return 0;
}

#include "shared.h"

typedef struct {
    int *items;
    size_t count;
    size_t capacity;
} Numbers;

int main(void)
{
    nob_log(INFO, "da_last:");
    Numbers xs = {0};
    for (int i = 12; i <= 16; ++i) da_append(&xs, i);
    while (xs.count > 0) {
        da_last(&xs) += 1;                 // as an rvalue
        printf("%d\n", da_last(&xs)); // as an lvalue
        xs.count--;
    }
    return 0;
}

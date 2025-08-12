#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

int main(void) 
{
    nob_log(INFO, "swap:");
    int a = 10;
    int b = 20;
    nob_log(INFO, "a = %d, b = %d", a, b);
    swap(a, b);
    nob_log(INFO, "a = %d, b = %d", a, b);

    return 0;
}

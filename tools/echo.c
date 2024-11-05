#define NOB_STRIP_PREFIX
#include "nob.h"

int main(int argc, char **argv)
{
    UNUSED(shift(argv, argc));
    if    (argc > 0)
    do    printf("%s", shift(argv, argc));
    while (argc > 0 && printf(" "));
    printf("\n");
    return 0;
}

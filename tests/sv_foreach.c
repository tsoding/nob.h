#include <stdio.h>

#define NOB_IMPLEMENTATION
#include "nob.h"

int main()
{
    String_View sv = SVLIT("Hello, World");
    da_foreach(const char, x, &sv) {
        printf("%c\n", *x);
    }
    return 0;
}

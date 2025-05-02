#include <stdio.h>
#include "foo.h"
#include "bar.h"

int main ()
{
    printf("Hello, World!\n");

    foo_hello();
    bar_hello();

    return 0;
}

#include <stdio.h>
#define BAZ_IMPLEMENTATION
#include "baz.h"

void foo_hello() 
{
    printf("[FOO] Hello from Foo!\n");
    printf("[FOO] Baz says:\n");
    int secret = baz_hello();
    printf("[FOO] And their secret number is %d\n", secret);
}

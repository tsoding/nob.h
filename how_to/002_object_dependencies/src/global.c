#include "foo.h"

#include <stdio.h>

struct foo global_foo = (struct foo){
    .bar = 0,
};

void frobnicate_global()
{
    frobnicate(&global_foo);
}

void print_global_status()
{
    printf("foo is %d\n", global_foo.bar);
}

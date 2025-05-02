#ifndef BAZ_H_
#define BAZ_H_

#include <stdio.h>

int baz_hello();

#endif // BAZ_H_

#ifdef BAZ_IMPLEMENTATION

int baz_hello()
{
    printf("[BAZ] Praise the sun!\n");
    return 42;
}

#endif // BAZ_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include "worker_utils.h"

int main(void)
{
    srand(get_seed());
    size_t loops = rand_double()*2*1000*1000*1000;
    for (size_t i = 0; i < loops; ++i);
    return 0;
}

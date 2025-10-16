#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "worker_utils.h"

int main()
{
    srand(get_seed());
    int ms = rand_float()*500;
    sleep_ms(ms);
    return 0;
}

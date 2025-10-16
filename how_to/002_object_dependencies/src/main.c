#include "global.h"

int main(void)
{
    frobnicate_global();
    print_global_status();

    frobnicate_global();
    print_global_status();
    return 0;
}

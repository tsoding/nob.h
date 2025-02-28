#include <stdio.h>
#include "config.h"

int main()
{
    printf("Hello, World\n");
#ifdef FOO
    printf("Foo feature\n");
#endif // FOO
#ifdef BAR
    printf("Bar feature\n");
#endif // BAR
    return 0;
}

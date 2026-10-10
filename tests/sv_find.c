#include "shared.h"

int main(void)
{
    printf("%d\n", sv_find(SVLIT("hello"), SVLIT("hello")));
    printf("%d\n", sv_find(SVLIT("hell"), SVLIT("hello")));
    printf("%d\n", sv_find(SVLIT("hello"), SVLIT("hell")));
    printf("%d\n", sv_find(SVLIT("hello, world"), SVLIT("hello")));
    printf("%d\n", sv_find(SVLIT("hello, world"), SVLIT("world")));
    printf("%d\n", sv_find(SVLIT("foo, bar"), SVLIT(",")));
    return 0;
}

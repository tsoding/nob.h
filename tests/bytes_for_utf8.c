#include <stdio.h>
#define NOB_IMPLEMENTATION
#include "nob.h"

int main()
{
    String_View sv = SVLIT("Привет, Мир!");
    size_t count = 0;
    while (sv.count > 0) {
        size_t n = bytes_for_utf8[(uint8_t)*sv.data];
        String_View c = sv_chop_left(&sv, n);
        printf(SV_Fmt" => %zu\n", SV_Arg(c), n);
        count += 1;
    }
    printf("count = %zu\n", count);
    return 0;
}

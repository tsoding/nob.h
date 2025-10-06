#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

int main()
{
    // line ending can be "\r\n" or "\n"
    const char *cstr = "one\r\ntwo\nthree\r\nfour\nfive\r\n";
    String_View sv   = sv_from_cstr(cstr);
    String_Views svs = sv_splitlines(sv);

    printf("line count: %zu\n", svs.count);
    for (size_t i = 0; i < svs.count; i++)
        printf("line %zu: \"%s\", char count: %zu\n", i+1, temp_sv_to_cstr(svs.items[i]), svs.items[i].count);

    da_free(svs);

    return 0;
}

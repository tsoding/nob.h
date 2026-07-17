#define NOB_IMPLEMENTATION
#include "nob.h"

String_View global_svlit = SVLIT_STATIC("Global SVLIT");

void param_svlit(String_View sv)
{
    printf(SV_Fmt"\n", SV_Arg(sv));
}

int main()
{
    String_View local_svlit = SVLIT("Local SVLIT");
    printf(SV_Fmt"\n", SV_Arg(global_svlit));
    printf(SV_Fmt"\n", SV_Arg(local_svlit));
    param_svlit(SVLIT("Parameter SVLIT"));
    return 0;
}

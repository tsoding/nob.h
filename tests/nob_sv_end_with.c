#include "shared.h"

void trace_case(String_View sv, const char *suffix)
{
    bool result = sv_end_with(sv, suffix);
    printf("sv_end_with(\"" SV_Fmt "\", \"%s\") => %s\n", SV_Arg(sv), suffix, result ? "true" : "false");
}

int main(void)
{
    String_View sv1 = sv_from_cstr("./example.exe");
    String_View sv2 = sv_from_cstr("");

    trace_case(sv1, "./example.exe");
    trace_case(sv1, ".exe");
    trace_case(sv1, "e");
    trace_case(sv1, "");
    trace_case(sv2, "");
    trace_case(sv1, ".png");
    trace_case(sv1, "/path/to/example.exe");
    trace_case(sv2, ".obj");

    return 0;
}

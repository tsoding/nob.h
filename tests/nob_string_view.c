#include "shared.h"

void trace_case(String_View sv, const char *suffix)
{
    bool result = sv_ends_with_cstr(sv, suffix);
    printf("sv_ends_with_cstr(\"" SV_Fmt "\", \"%s\") => %s\n", SV_Arg(sv), suffix, result ? "true" : "false");
}

int main(void)
{
    {
        printf("-- sv_end_with --\n");
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
    }

    {
        printf("-- sv_chop_prefix --\n");
        String_View sv     = sv_from_cstr("hello world");
        String_View prefix = sv_from_cstr("foo");
        bool result = sv_chop_prefix(&sv, prefix);
        printf("sv = \"" SV_Fmt "\", prefix = \"" SV_Fmt "\", result = %s\n", SV_Arg(sv), SV_Arg(prefix), result ? "true" : "false");

        prefix = sv_from_cstr("hello");
        result = sv_chop_prefix(&sv, prefix);
        printf("sv = \"" SV_Fmt "\", prefix = \"" SV_Fmt "\", result = %s\n", SV_Arg(sv), SV_Arg(prefix), result ? "true" : "false");
    }

    {
        printf("-- sv_chop_suffix --\n");
        String_View sv     = sv_from_cstr("hello world");
        String_View suffix = sv_from_cstr("foo");
        bool result = sv_chop_suffix(&sv, suffix);
        printf("sv = \"" SV_Fmt "\", suffix = \"" SV_Fmt "\", result = %s\n", SV_Arg(sv), SV_Arg(suffix), result ? "true" : "false");

        suffix = sv_from_cstr("world");
        result = sv_chop_suffix(&sv, suffix);
        printf("sv = \"" SV_Fmt "\", suffix = \"" SV_Fmt "\", result = %s\n", SV_Arg(sv), SV_Arg(suffix), result ? "true" : "false");
    }

    return 0;
}

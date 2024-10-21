#define NOB_IMPLEMENTATION
#include "nob.h"

void assert_true(const char *test_case, bool result)
{
    if (result) {
        nob_log(NOB_INFO, "[SUCCESS] %s", test_case);
    } else {
        nob_log(NOB_ERROR, "[FAIL] %s", test_case);
    }
}

void assert_false(const char *test_case, bool result)
{
    if (!result) {
        nob_log(NOB_INFO, "[SUCCESS] %s", test_case);
    } else {
        nob_log(NOB_ERROR, "[FAIL] %s", test_case);
    }
}

int main(void)
{
    Nob_String_View sv1 = nob_sv_from_cstr("./example.exe");
    Nob_String_View sv2 = nob_sv_from_cstr("");

    assert_true("nob_sv_end_with(sv1, \"./example.exe\")", nob_sv_end_with(sv1, "./example.exe"));
    assert_true("nob_sv_end_with(sv1, \".exe\")", nob_sv_end_with(sv1, ".exe"));
    assert_true("nob_sv_end_with(sv1, \"e\")", nob_sv_end_with(sv1, "e"));
    assert_true("nob_sv_end_with(sv1, \"\")", nob_sv_end_with(sv1, ""));
    assert_true("nob_sv_end_with(sv2, \"\")", nob_sv_end_with(sv2, ""));

    assert_false("nob_sv_end_with(sv1, \".png\")", nob_sv_end_with(sv1, ".png"));
    assert_false("nob_sv_end_with(sv1, \"/path/to/example.exe\")", nob_sv_end_with(sv1, "/path/to/example.exe"));
    assert_false("nob_sv_end_with(sv2, \".obj\")", nob_sv_end_with(sv2, ".obj"));

    return 0;
}

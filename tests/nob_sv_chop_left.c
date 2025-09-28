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
    Nob_String_View sv_chop_left[] = {
        nob_sv_from_cstr("foobar"),
        nob_sv_from_cstr("yet_another_foobar"),
        nob_sv_from_cstr("example"),
        nob_sv_from_cstr("another_example"),
    };

    assert_true("nob_sv_chop_left(sv1, \"foobar\", 2)", nob_sv_eq(nob_sv_chop_left(&sv_chop_left[0], 2), nob_sv_from_cstr("obar")));
    assert_false("nob_sv_chop_left(sv2, \"yet_another_foobar\", 4)", nob_sv_eq(nob_sv_chop_left(&sv_chop_left[1], 4), nob_sv_from_cstr("obar")));
    assert_true("nob_sv_chop_left(sv3, \"example\", 3)", nob_sv_eq(nob_sv_chop_left(&sv_chop_left[2], 3), nob_sv_from_cstr("mple")));
    assert_false("nob_sv_chop_left(sv4, \"another_example\", 3)", nob_sv_eq(nob_sv_chop_left(&sv_chop_left[3], 3), nob_sv_from_cstr("ampl")));

    return 0;
}
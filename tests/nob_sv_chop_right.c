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
    Nob_String_View sv_chop_right[] = {
        nob_sv_from_cstr("foobar"),
        nob_sv_from_cstr("yet_another_foobar"),
        nob_sv_from_cstr("example"),
        nob_sv_from_cstr("another_example"),
    };

    assert_true("nob_sv_chop_right(sv1, \"foobar\", 2)", nob_sv_eq(nob_sv_chop_right(&sv_chop_right[0], 2), nob_sv_from_cstr("fo")));
    assert_false("nob_sv_chop_right(sv2, \"yet_another_foobar\", 4)", nob_sv_eq(nob_sv_chop_right(&sv_chop_right[1], 4), nob_sv_from_cstr("obar")));
    assert_true("nob_sv_chop_right(sv3, \"example\", 4)", nob_sv_eq(nob_sv_chop_right(&sv_chop_right[2], 4), nob_sv_from_cstr("exam")));
    assert_false("nob_sv_chop_right(sv4, \"another_example\", 4)", nob_sv_eq(nob_sv_chop_right(&sv_chop_right[3], 4), nob_sv_from_cstr("")));

    return 0;
}
#define NOB_IMPLEMENTATION
#include "nob.h"

#define BUILD_FOLDER "build/"
#define TESTS_FOLDER "tests/"

const char *test_names[] = {
    "minimal_log_level",
};
#define test_names_count NOB_ARRAY_LEN(test_names)

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;

    // TODO: Implement record/replay testing
    Nob_Cmd cmd = {0};
    for (size_t i = 0; i < test_names_count; ++i) {
        const char *bin_path = nob_temp_sprintf("%s%s", BUILD_FOLDER, test_names[i]);
        const char *src_path = nob_temp_sprintf("%s%s.c", TESTS_FOLDER, test_names[i]);
        nob_cmd_append(&cmd, "cc", "-I.", "-o", bin_path, src_path);
        if (!nob_cmd_run_sync_and_reset(&cmd)) return 1;
        nob_cmd_append(&cmd, bin_path);
        if (!nob_cmd_run_sync_and_reset(&cmd)) return 1;
        nob_log(NOB_INFO, "--- %s finished ---", bin_path);
    }

    return 0;
}

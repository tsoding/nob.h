#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#define BUILD_FOLDER "build/"
#define TESTS_FOLDER "tests/"

const char *test_names[] = {
    "minimal_log_level",
    "nob_sv_end_with",
#ifdef _WIN32
    "win32_error",
#endif //_WIN32
};
#define test_names_count ARRAY_LEN(test_names)

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!mkdir_if_not_exists(BUILD_FOLDER)) return 1;

    // TODO: Implement record/replay testing
    Cmd cmd = {0};
    for (size_t i = 0; i < test_names_count; ++i) {
        const char *bin_path = temp_sprintf("%s%s", BUILD_FOLDER, test_names[i]);
        const char *src_path = temp_sprintf("%s%s.c", TESTS_FOLDER, test_names[i]);
        cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-Wswitch-enum", "-I.", "-o", bin_path, src_path);
        if (!cmd_run_sync_and_reset(&cmd)) return 1;
        cmd_append(&cmd, bin_path);
        if (!cmd_run_sync_and_reset(&cmd)) return 1;
        nob_log(INFO, "--- %s finished ---", bin_path);
    }

    return 0;
}

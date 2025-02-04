#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"
#include "shared.h"

const char *test_names[] = {
    "minimal_log_level",
    "nob_sv_end_with",
    "set_get_current_dir",
    "cmd_redirect",
#ifdef _WIN32
    "win32_error",
#endif //_WIN32
    "read_entire_dir",
};
#define test_names_count ARRAY_LEN(test_names)

bool build_and_run_test(Cmd *cmd, const char *test_name)
{
    const char *bin_path = temp_sprintf("%s%s", BUILD_FOLDER TESTS_FOLDER, test_name);
    const char *src_path = temp_sprintf("%s%s.c", TESTS_FOLDER, test_name);
    cmd_append(cmd, "cc", "-Wall", "-Wextra", "-Wswitch-enum", "-I.", "-o", bin_path, src_path);
    if (!cmd_run_sync_and_reset(cmd)) return false;
    cmd_append(cmd, bin_path);
    if (!cmd_run_sync_and_reset(cmd)) return false;
    nob_log(INFO, "--- %s finished ---", bin_path);
    return true;
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    Cmd cmd = {0};

    const char *program_name = shift(argv, argc);
    const char *command_name = "test";
    if (argc > 0) command_name = shift(argv, argc);

    if (!mkdir_if_not_exists(BUILD_FOLDER)) return 1;
    if (!mkdir_if_not_exists(BUILD_FOLDER TESTS_FOLDER)) return 1;
    if (!mkdir_if_not_exists(BUILD_FOLDER TOOLS_FOLDER)) return 1;

    if (strcmp(command_name, "test") == 0) {
        if (argc <= 0) {
            for (size_t i = 0; i < test_names_count; ++i) {
                if (!build_and_run_test(&cmd, test_names[i])) return 1;
            }
            return 0;
        }

        while (argc > 0) {
            const char *test_name = shift(argv, argc);
            if (!build_and_run_test(&cmd, test_name)) return 1;
        }
        return 0;
    }

    if (strcmp(command_name, "list") == 0) {
        nob_log(INFO, "Tests:");
        for (size_t i = 0; i < test_names_count; ++i) {
            nob_log(INFO, "    %s", test_names[i]);
        }
        nob_log(INFO, "Use %s test <names...> to run individual tests", program_name);
        return 0;
    }

    nob_log(ERROR, "Unknown command %s", command_name);
    return 1;
}

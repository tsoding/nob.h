#include "shared.h"
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "nob.h"
#undef rename                   // Testing for backward compatibility after v1.20.6

const char *test_names[] = {
    "minimal_log_level",
    "nob_sv_end_with",
    "set_get_current_dir",
    "cmd_redirect",
    "cmd_args_passing",
#ifdef _WIN32
    "win32_error",
#endif //_WIN32
    "read_entire_dir",
    "da_resize",
    "da_last",
    "da_remove_unordered",
    "da_append",
    "sb_appendf",
    "da_foreach",
};
#define test_names_count ARRAY_LEN(test_names)

bool build_and_run_test(Cmd *cmd, const char *test_name)
{
    size_t mark = temp_save();

    const char *bin_path = temp_sprintf("%s%s", BUILD_FOLDER TESTS_FOLDER, test_name);
    const char *src_path = temp_sprintf("%s%s.c", TESTS_FOLDER, test_name);
    nob_cc(cmd);
    nob_cc_flags(cmd);
    nob_cc_output(cmd, bin_path);
    nob_cc_inputs(cmd, src_path);
    if (!cmd_run_sync_and_reset(cmd)) return false;

    const char *test_cwd_path = temp_sprintf("%s%s%s.cwd", BUILD_FOLDER, TESTS_FOLDER, test_name);
    if (!mkdir_if_not_exists(test_cwd_path)) return false;
    if (!set_current_dir(test_cwd_path)) return false;
    cmd_append(cmd, temp_sprintf("../%s", test_name));
    if (!cmd_run_sync_and_reset(cmd))    return false;
    if (!set_current_dir("../../../")) return false;

    nob_log(INFO, "--- %s finished ---", bin_path);

    temp_rewind(mark);
    return true;
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF_PLUS(argc, argv, "nob.h", "shared.h");

    Cmd cmd = {0};

    const char *program_name = shift(argv, argc);
    const char *command_name = "test";
    if (argc > 0) command_name = shift(argv, argc);

    if (!mkdir_if_not_exists(BUILD_FOLDER)) return 1;
    if (!mkdir_if_not_exists(BUILD_FOLDER TESTS_FOLDER)) return 1;

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

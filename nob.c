#include "shared.h"
#define NOB_IMPLEMENTATION
#define NOB_ADD_PREFIX
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "nob.h"

const char *test_names[] = {
    "minimal_log_level",
    "nob_sv_end_with",
    "set_get_current_dir",
    "cmd_redirect",
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
#define test_names_count NOB_ARRAY_LEN(test_names)

bool build_and_run_test(Nob_Cmd *cmd, const char *test_name)
{
    size_t mark = nob_temp_save();

    const char *bin_path = nob_temp_sprintf("%s%s", BUILD_FOLDER TESTS_FOLDER, test_name);
    const char *src_path = nob_temp_sprintf("%s%s.c", TESTS_FOLDER, test_name);
    nob_cc(cmd);
    nob_cc_flags(cmd);
    nob_cc_output(cmd, bin_path);
    nob_cc_inputs(cmd, src_path);
    if (!nob_cmd_run_sync_and_reset(cmd)) return false;

    const char *test_cwd_path = nob_temp_sprintf("%s%s%s.cwd", BUILD_FOLDER, TESTS_FOLDER, test_name);
    if (!nob_mkdir_if_not_exists(test_cwd_path)) return false;
    if (!nob_set_current_dir(test_cwd_path)) return false;
    nob_cmd_append(cmd, nob_temp_sprintf("../%s", test_name));
    if (!nob_cmd_run_sync_and_reset(cmd))    return false;
    if (!nob_set_current_dir("../../../")) return false;

    nob_log(NOB_INFO, "--- %s finished ---", bin_path);

    nob_temp_rewind(mark);
    return true;
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF_PLUS(argc, argv, "nob.h", "shared.h");

    Nob_Cmd cmd = {0};

    const char *program_name = nob_shift(argv, argc);
    const char *command_name = "test";
    if (argc > 0) command_name = nob_shift(argv, argc);

    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;
    if (!nob_mkdir_if_not_exists(BUILD_FOLDER TESTS_FOLDER)) return 1;

    if (strcmp(command_name, "test") == 0) {
        if (argc <= 0) {
            for (size_t i = 0; i < test_names_count; ++i) {
                if (!build_and_run_test(&cmd, test_names[i])) return 1;
            }
            return 0;
        }

        while (argc > 0) {
            const char *test_name = nob_shift(argv, argc);
            if (!build_and_run_test(&cmd, test_name)) return 1;
        }
        return 0;
    }

    if (strcmp(command_name, "list") == 0) {
        nob_log(NOB_INFO, "Tests:");
        for (size_t i = 0; i < test_names_count; ++i) {
            nob_log(NOB_INFO, "    %s", test_names[i]);
        }
        nob_log(NOB_INFO, "Use %s test <names...> to run individual tests", program_name);
        return 0;
    }

    nob_log(NOB_ERROR, "Unknown command %s", command_name);
    return 1;
}


#include "shared.h"
#define NOBDEF static inline
#define NOB_IMPLEMENTATION
#define NOB_EXPERIMENTAL_DELETE_OLD
#define NOB_WARN_DEPRECATED
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
    "temp_aligned_alloc",
    "temp_path_comps",
    "temp_running_executable_path",
    "no_echo",
    "cmd_run_dont_reset",
};
#define test_names_count ARRAY_LEN(test_names)

bool build_and_run_test(Cmd *cmd, const char *test_name, bool record)
{
    size_t mark = temp_save();

    const char *bin_path = temp_sprintf("%s%s", BUILD_FOLDER TESTS_FOLDER, test_name);
    const char *src_path = temp_sprintf("%s%s.c", TESTS_FOLDER, test_name);
    nob_cc(cmd);
    nob_cc_flags(cmd);
    nob_cc_output(cmd, bin_path);
    nob_cc_inputs(cmd, src_path);
    if (!cmd_run(cmd)) return false;

    const char *test_cwd_path = temp_sprintf("%s%s%s.cwd", BUILD_FOLDER, TESTS_FOLDER, test_name);
    if (!mkdir_if_not_exists(test_cwd_path)) return false;
    if (!set_current_dir(test_cwd_path)) return false;
    cmd_append(cmd, temp_sprintf("../%s", test_name));
    const char *test_stdout_path = temp_sprintf("../%s.stdout.txt", test_name);
    const char *test_stderr_path = temp_sprintf("../%s.stderr.txt", test_name);
    if (!cmd_run(cmd, .stdout_path = test_stdout_path, .stderr_path = test_stderr_path)) return false;
    if (!set_current_dir("../../../")) return false;

    const char *src_stdout_path = temp_sprintf("%s%s.stdout.txt", BUILD_FOLDER TESTS_FOLDER, test_name);
    const char *src_stderr_path = temp_sprintf("%s%s.stderr.txt", BUILD_FOLDER TESTS_FOLDER, test_name);
    const char *dst_stdout_path = temp_sprintf("%s%s.stdout.txt", TESTS_FOLDER, test_name);
    const char *dst_stderr_path = temp_sprintf("%s%s.stderr.txt", TESTS_FOLDER, test_name);
    if (record) {
        if (!copy_file(src_stdout_path, dst_stdout_path)) return 1;
        if (!copy_file(src_stderr_path, dst_stderr_path)) return 1;
    } else {
        cmd_append(cmd, "diff");
        cmd_append(cmd, "-u");
        cmd_append(cmd, src_stdout_path);
        cmd_append(cmd, dst_stdout_path);
        if (!cmd_run(cmd)) return false;

        cmd_append(cmd, "diff");
        cmd_append(cmd, "-u");
        cmd_append(cmd, src_stderr_path);
        cmd_append(cmd, dst_stderr_path);
        if (!cmd_run(cmd)) return false;
    }

    nob_log(INFO, "--- %s finished ---", bin_path);

    temp_rewind(mark);
    return true;
}

typedef struct {
    const char *name;
    const char *signature;
    const char *description;
} Command;

typedef struct {
    Command *items;
    size_t count;
    size_t capacity;
} Commands;

bool command(const char *arg, Commands *commands, const char *name, const char *signature, const char *description)
{
    Command command = {
        .name = name,
        .signature = signature,
        .description = description,
    };
    da_append(commands, command);
    return strcmp(arg, name) == 0;
}

void print_available_commands(Commands commands)
{
    size_t max_name_width = 0;
    size_t max_sign_width = 0;
    da_foreach(Command, command, &commands) {
        size_t name_width = strlen(command->name);
        size_t sign_width = strlen(command->signature);
        if (name_width > max_name_width) max_name_width = name_width;
        if (sign_width > max_sign_width) max_sign_width = sign_width;
    }
    nob_log(INFO, "Available commands:");
    da_foreach(Command, command, &commands) {
        nob_log(INFO, "    %-*s %-*s - %s", (int)max_name_width, command->name, (int)max_sign_width, command->signature, command->description);
    }
}

int main(int argc, char **argv)
{
    set_log_handler(cancer_log_handler);

    GO_REBUILD_URSELF_PLUS(argc, argv, "nob.h", "shared.h");

    Cmd cmd = {0};

    const char *program_name = shift(argv, argc);
    const char *command_name = "test";
    if (argc > 0) command_name = shift(argv, argc);

    Commands commands = {0};

    if (command(command_name, &commands, "test", "[test_names...]", "Run the tests checking their expected output")) {
        if (!mkdir_if_not_exists(BUILD_FOLDER)) return 1;
        if (!mkdir_if_not_exists(BUILD_FOLDER TESTS_FOLDER)) return 1;

        if (argc <= 0) {
            for (size_t i = 0; i < test_names_count; ++i) {
                if (!build_and_run_test(&cmd, test_names[i], false)) return 1;
            }
            return 0;
        }

        while (argc > 0) {
            const char *test_name = shift(argv, argc);
            if (!build_and_run_test(&cmd, test_name, false)) return 1;
        }
        return 0;
    }

    if (command(command_name, &commands, "record", "[test_names...]", "Record expected output of the tests")) {
        if (!mkdir_if_not_exists(BUILD_FOLDER)) return 1;
        if (!mkdir_if_not_exists(BUILD_FOLDER TESTS_FOLDER)) return 1;

        if (argc <= 0) {
            for (size_t i = 0; i < test_names_count; ++i) {
                if (!build_and_run_test(&cmd, test_names[i], true)) return 1;
            }
            return 0;
        }

        while (argc > 0) {
            const char *test_name = shift(argv, argc);
            if (!build_and_run_test(&cmd, test_name, true)) return 1;
        }
        return 0;
    }

    if (command(command_name, &commands, "list", "", "List available tests")) {
        nob_log(INFO, "Tests:");
        for (size_t i = 0; i < test_names_count; ++i) {
            nob_log(INFO, "    %s", test_names[i]);
        }
        nob_log(INFO, "Use %s test <names...> to run individual tests", program_name);
        return 0;
    }

    if (command(command_name, &commands, "help", "", "Print this help message")) {
        print_available_commands(commands);
        return 0;
    }

    print_available_commands(commands);
    nob_log(ERROR, "Unknown command %s", command_name);
    return 1;
}

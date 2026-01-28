#include "shared.h"

const char *test_names[] = {
    "nob_sv_end_with",
    "cmd_redirect",
    "cmd_args_passing",
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
    "chain",
    "private_functions_inside_public_macros",
};
#define test_names_count ARRAY_LEN(test_names)

bool delete_walk_entry(Walk_Entry entry)
{
    return delete_file(entry.path);
}

// TODO: we eventually should make this part of nob.h
bool delete_directory_recursively(const char *dir_path)
{
    return walk_dir(dir_path, delete_walk_entry, .post_order = true);
}

bool build_and_run_test(const char *test_name, bool record)
{
    bool result = true;
    Cmd cmd = {0};
    String_Builder src = {0};
    String_Builder dst = {0};
    String_View src_sv = {0};
    String_View dst_sv = {0};
#ifdef _WIN32
    const char *src_stdout_path  = temp_sprintf("%s%s.win32.stdout.txt", BUILD_FOLDER TESTS_FOLDER, test_name);
    const char *dst_stdout_path  = temp_sprintf("%s%s.win32.stdout.txt", TESTS_FOLDER, test_name);
    const char *test_stdout_path = temp_sprintf("../%s.win32.stdout.txt", test_name);
#else
    const char *src_stdout_path  = temp_sprintf("%s%s.stdout.txt", BUILD_FOLDER TESTS_FOLDER, test_name);
    const char *dst_stdout_path  = temp_sprintf("%s%s.stdout.txt", TESTS_FOLDER, test_name);
    const char *test_stdout_path = temp_sprintf("../%s.stdout.txt", test_name);
#endif // _WIN32
    const char *bin_path         = temp_sprintf("%s%s", BUILD_FOLDER TESTS_FOLDER, test_name);
    const char *src_path         = temp_sprintf("%s%s.c", TESTS_FOLDER, test_name);
    const char *test_cwd_path    = temp_sprintf("%s%s%s.cwd", BUILD_FOLDER, TESTS_FOLDER, test_name);

    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    nob_cc_output(&cmd, bin_path);
    nob_cc_inputs(&cmd, src_path);
    if (!cmd_run(&cmd)) return_defer(false);

    if (file_exists(test_cwd_path)) {
        if (!delete_directory_recursively(test_cwd_path)) return_defer(false);
    }
    if (!mkdir_if_not_exists(test_cwd_path)) return_defer(false);
    if (!set_current_dir(test_cwd_path)) return_defer(false);
#ifdef _WIN32
    cmd_append(&cmd, temp_sprintf("../%s.exe", test_name));
#else
    cmd_append(&cmd, temp_sprintf("../%s", test_name));
#endif // _WIN32
    if (!cmd_run(&cmd, .stdout_path = test_stdout_path)) return_defer(false);
    if (!set_current_dir("../../../")) return_defer(false);

    // TODO: implement record/replay testing for windows
    if (record) {
        if (!copy_file(src_stdout_path, dst_stdout_path)) return_defer(false);
    } else {
        // TODO: it would be cool to have a portable diff utility in here.
        if (!read_entire_file(src_stdout_path, &src)) return_defer(false);
        if (!read_entire_file(dst_stdout_path, &dst)) return_defer(false);

        src_sv = sb_to_sv(src);
        dst_sv = sb_to_sv(dst);

        if (!sv_eq(src_sv, dst_sv)) {
            nob_log(ERROR, "UNEXPECTED OUTPUT!");
            nob_log(ERROR, "EXPECTED:");
            fprintf(stderr, SV_Fmt, SV_Arg(dst_sv));
            nob_log(ERROR, "ACTUAL:");
            fprintf(stderr, SV_Fmt, SV_Arg(src_sv));
            return_defer(false);
        }
    }

    nob_log(INFO, "--- %s finished ---", bin_path);

defer:
    free(dst.items);
    free(src.items);
    free(cmd.items);
    return result;
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

    bool picked;
    const char *picked_name;
    const char *picked_at_file;
    int picked_at_line;
} Commands;

void commands_reset(Commands *commands)
{
    commands->count = 0;
    commands->picked = false;
}

#define command(arg, commands, name, signature, description) command_loc(__FILE__, __LINE__, (arg), (commands), (name), (signature), (description))
bool command_loc(const char *file, int line, const char *arg, Commands *commands, const char *name, const char *signature, const char *description)
{
    if (commands->picked) {
        fprintf(stderr, "%s:%d: ASSERTION FAILED: the branch for command `%s` fell through.\n", commands->picked_at_file, commands->picked_at_line, commands->picked_name);
        fprintf(stderr, "%s:%d: NOTE: the execution proceeded to here, but the command was already picked.\n", file, line);
        abort();
    }
    Command command = {
        .name = name,
        .signature = signature,
        .description = description,
    };
    da_append(commands, command);
    commands->picked_name    = name;
    commands->picked_at_line = line;
    commands->picked_at_file = file;
    commands->picked         = (strcmp(arg, name) == 0);
    return commands->picked;
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
    GO_REBUILD_URSELF_PLUS(argc, argv, "nob.h", "shared.h");

    Cmd cmd = {0};

    const char *program_name = shift(argv, argc);
    const char *command_name = "test";
    if (argc > 0) command_name = shift(argv, argc);

    Commands commands = {0};

    commands_reset(&commands);

    if (command(command_name, &commands, "test", "[test_names...]", "Run the tests checking their expected output")) {
        if (!mkdir_if_not_exists(BUILD_FOLDER)) return 1;
        if (!mkdir_if_not_exists(BUILD_FOLDER TESTS_FOLDER)) return 1;

        size_t failed_count = 0;
        if (argc <= 0) {
            for (size_t i = 0; i < test_names_count; ++i) {
                size_t mark = temp_save();
                if (!build_and_run_test(test_names[i], false)) failed_count += 1;
                temp_rewind(mark);
            }
        } else {
            while (argc > 0) {
                size_t mark = temp_save();
                const char *test_name = shift(argv, argc);
                if (!build_and_run_test(test_name, false)) failed_count += 1;
                temp_rewind(mark);
            }
        }
        if (failed_count > 0) return 1;

        return 0;
    }

    if (command(command_name, &commands, "record", "[test_names...]", "Record expected output of the tests")) {
        if (!mkdir_if_not_exists(BUILD_FOLDER)) return 1;
        if (!mkdir_if_not_exists(BUILD_FOLDER TESTS_FOLDER)) return 1;

        size_t failed_count = 0;
        if (argc <= 0) {
            for (size_t i = 0; i < test_names_count; ++i) {
                size_t mark = temp_save();
                if (!build_and_run_test(test_names[i], true)) failed_count += 1;
                temp_rewind(mark);
            }
        } else {
            while (argc > 0) {
                size_t mark = temp_save();
                const char *test_name = shift(argv, argc);
                if (!build_and_run_test(test_name, true)) failed_count += 1;
                temp_rewind(mark);
            }
        }
        if (failed_count > 0) return 1;

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

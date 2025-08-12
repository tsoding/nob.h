#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

int main(void)
{
    Cmd cmd = {0};

    const char *print_args_src =
        "#include <stdio.h>\n"
        "\n"
        "int main(int argc, char **argv) {\n"
        "    for (int i = 1; i < argc; ++i) {\n"
        "        printf(\"%d: %s\\n\", i, argv[i]);\n"
        "    }\n"
        "    return 0;\n"
        "}\n";
    if (!write_entire_file("print_args.c", print_args_src, strlen(print_args_src))) return 1;

    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    nob_cc_output(&cmd, "print_args");
    nob_cc_inputs(&cmd, "print_args.c");
    if (!cmd_run_sync_and_reset(&cmd)) return 1;

    cmd_append(&cmd, "./print_args");
    cmd_append(&cmd, "foo");
    cmd_append(&cmd, "bar");
    cmd_append(&cmd, "Hello, world");
    cmd_append(&cmd, "\"Hello, world\"");
    cmd_append(&cmd, "\"\\` %$*@");
    if (!cmd_run_sync_and_reset(&cmd)) return 1;

    return 0;
}

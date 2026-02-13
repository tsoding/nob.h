#include "shared.h"

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
    if (!build_tool(&cmd, NULL, "print_args", "print_args.c", print_args_src)) return 1;

    cmd_append(&cmd, "./print_args");
    cmd_append(&cmd, "foo");
    cmd_append(&cmd, "bar");
    cmd_append(&cmd, "");
    cmd_append(&cmd, NULL);
    cmd_append(&cmd, "Hello, world");
    cmd_append(&cmd, "\"Hello, world\"");
    cmd_append(&cmd, "\"\\` %$*@");
    if (!cmd_run(&cmd)) return 1;

    return 0;
}

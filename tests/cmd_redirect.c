#include "shared.h"

int main(void)
{
    int result = 0;

    Cmd cmd = {0};
    String_Builder sb = {0};
    String_View actual_message = {0};
    const char *message = NULL;
    const char *message_file_path = NULL;

    const char *echo_src =
        "#include <assert.h>\n"
        "#include <stdio.h>\n"
        "\n"
        "#define shift(xs, xs_sz) (assert((xs_sz) > 0), (xs_sz)--, *(xs)++)\n"
        "\n"
        "int main(int argc, char **argv)\n"
        "{\n"
        "    (void)shift(argv, argc);\n"
        "    if    (argc > 0)\n"
        "    do    printf(\"%s\", shift(argv, argc));\n"
        "    while (argc > 0 && printf(\" \"));\n"
        "    printf(\"\\n\");\n"
        "    return 0;\n"
        "}\n";
    if (!build_tool(&cmd, NULL, "echo", "echo.c", echo_src)) return 1;

    message = "Redirected message";
    message_file_path = "./echo_message.txt";

    cmd_append(&cmd, "./echo", message);
    if (!cmd_run(&cmd, .stdout_path = message_file_path)) return 1;

    if (!read_entire_file(message_file_path, &sb)) return 1;
    actual_message = sb_to_sv(sb);
    printf(SV_Fmt, SV_Arg(actual_message));

    return result;
}

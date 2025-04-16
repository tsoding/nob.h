#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

int main(void)
{
    int result = 0;

    Cmd cmd = {0};
    Fd fdout = INVALID_FD;
    String_Builder sb = {0};

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
    if (!write_entire_file("./echo.c", echo_src, strlen(echo_src))) return 1;

    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    nob_cc_output(&cmd, "./echo");
    nob_cc_inputs(&cmd, "./echo.c");
    if (!cmd_run_sync_and_reset(&cmd)) return_defer(1);

    // TODO: fix the rendering of the shell command on Windows.
    //   It prevents use from using the message with spaces here.
    //   Steal some code from C3 compiler, I already implemented it there.
    // const char *message = "Hello, World";
    const char *message = "Hello";
    const char *message_file_path = "./echo_message.txt";

    fdout = fd_open_for_write(message_file_path);
    if (fdout == INVALID_FD) return_defer(1);

    cmd_append(&cmd, "./echo", message);
    Proc p = cmd_run_async_redirect_and_reset(&cmd, (Cmd_Redirect) {.fdout = &fdout});
    if (p == INVALID_PROC) return_defer(1);
    if (!proc_wait(p)) return_defer(1);

    if (!read_entire_file(message_file_path, &sb)) return_defer(1);
    String_View actual_message = sb_to_sv(sb);
    if (!sv_eq(sv_trim(actual_message), sv_from_cstr(message))) {
        nob_log(ERROR, "Unexpected message");
        nob_log(ERROR, "Expected: %s", message);
        nob_log(ERROR, "Actual:   "SV_Fmt, SV_Arg(actual_message));
        return_defer(1);
    }

    nob_log(INFO, "OK");

defer:
    free(cmd.items);
    free(sb.items);
    if (fdout != INVALID_FD) fd_close(fdout);
    return result;
}

#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

int main()
{
    Cmd cmd = {0};

    const char *empty_src = "int main() { return 0; }";
    if (!write_entire_file("empty.c", empty_src, strlen(empty_src))) return 1;

    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    nob_cc_output(&cmd, "empty");
    nob_cc_inputs(&cmd, "empty.c");
    if (!cmd_run(&cmd)) return 1;

    nob_log(INFO, "Reset:");
    for (size_t i = 0; i < cmd.count; ++i) {
        nob_log(INFO, "  %s", cmd.items[i]);
    }

    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    nob_cc_output(&cmd, "empty");
    nob_cc_inputs(&cmd, "empty.c");
    if (!cmd_run(&cmd, .dont_reset = true)) return 1;

    nob_log(INFO, "Don't Reset:");
    for (size_t i = 0; i < cmd.count; ++i) {
        nob_log(INFO, "  %s", cmd.items[i]);
    }

    return 0;
}

#define NOB_IMPLEMENTATION
#define NOB_NO_ECHO
#include "nob.h"

int main(void)
{
    Cmd cmd = {0};

    const char *empty_src = "int main() { return 0; }";
    if (!write_entire_file("empty.c", empty_src, strlen(empty_src))) return 1;

    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    nob_cc_output(&cmd, "empty");
    nob_cc_inputs(&cmd, "empty.c");
    if (!cmd_run(&cmd)) return 1;

    cmd_append(&cmd, "./empty");
    if (!cmd_run(&cmd)) return 1;

    return 0;
}

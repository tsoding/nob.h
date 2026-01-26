#include "shared.h"

int main()
{
    Cmd cmd = {0};

    const char *empty_src = "int main() { return 0; }";
    if (!build_tool(&cmd, NULL, "./empty", "./empty.c", empty_src)) return 1;

    cmd_append(&cmd, "./empty", "foo", "bar", "baz");
    if (!cmd_run(&cmd)) return 1;
    printf("Reset:\n");
    for (size_t i = 0; i < cmd.count; ++i) {
        printf("  %s\n", cmd.items[i]);
    }

    cmd_append(&cmd, "./empty", "foo", "bar", "baz");
    if (!cmd_run(&cmd, .dont_reset = true)) return 1;
    printf("Don't Reset:\n");
    for (size_t i = 0; i < cmd.count; ++i) {
        printf("  %s\n", cmd.items[i]);
    }

    return 0;
}

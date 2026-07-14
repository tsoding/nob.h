#define NOB_NO_ECHO
#include "shared.h"

size_t log_count = 0;

void counting_log_handler(Nob_Log_Level level, const char *fmt, va_list args)
{
    UNUSED(level);
    UNUSED(fmt);
    UNUSED(args);
    log_count += 1;
}

int main(void)
{
    set_log_handler(counting_log_handler);

    Cmd cmd = {0};

    const char *empty_src = "int main() { return 0; }";
    if (!build_tool(&cmd, NULL, "./empty", "./empty.c", empty_src)) return 1;

    cmd_append(&cmd, "./empty");
    if (!cmd_run(&cmd)) return 1;

    printf("Log entries issued: %zu\n", log_count);

    return 0;
}

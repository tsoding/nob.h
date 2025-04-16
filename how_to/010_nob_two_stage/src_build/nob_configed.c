#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"
#include "config.h"
#include "folders.h"

int main(void)
{
    Cmd cmd = {0};
#ifdef FOO
    nob_log(INFO, "FOO feature is enabled");
#endif // FOO
#ifdef BAR
    nob_log(INFO, "BAR feature is enabled");
#endif // BAR
    const char *output_path = BUILD_FOLDER"main";
    const char *input_path = SRC_FOLDER"main.c";
    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    nob_cmd_append(&cmd, "-I"BUILD_FOLDER, "-I.");
    nob_cc_output(&cmd, output_path);
    nob_cc_inputs(&cmd, input_path);
    if (!cmd_run_sync_and_reset(&cmd)) return 1;
    return 0;
}

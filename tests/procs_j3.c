#define NOB_IMPLEMENTATION
#include "nob.h"

int main(void)
{
    Nob_Cmd cmd = {0};

    Nob_Procs procs = {0};
    nob_da_resize(&procs, 3);
    procs.count = 0;

    for (size_t i = 0; i < 10; ++i) {
        nob_cmd_append(&cmd, "echo", nob_temp_sprintf("running %d-nth command", i));
        nob_procs_append_or_wait_and_reset(&procs, nob_cmd_run_async_and_reset(&cmd));
    }
    if (!nob_procs_wait_and_reset(&procs)) return 1;

    return 0;
}

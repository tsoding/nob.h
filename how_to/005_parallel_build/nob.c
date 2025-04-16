#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#define BUILD_FOLDER "build/"
#define SRC_FOLDER "src/"

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    Cmd cmd = {0};
    Procs procs = {0};

    if (!mkdir_if_not_exists(BUILD_FOLDER)) return 1;

    // Spawn three async processes collecting them to procs dynamic array
    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    nob_cc_output(&cmd, BUILD_FOLDER"foo");
    nob_cc_inputs(&cmd, SRC_FOLDER"foo.c");
    da_append(&procs, cmd_run_async_and_reset(&cmd));

    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    nob_cc_output(&cmd, BUILD_FOLDER"bar");
    nob_cc_inputs(&cmd, SRC_FOLDER"bar.c");
    da_append(&procs, cmd_run_async_and_reset(&cmd));

    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    nob_cc_output(&cmd, BUILD_FOLDER"baz");
    nob_cc_inputs(&cmd, SRC_FOLDER"baz.c");
    da_append(&procs, cmd_run_async_and_reset(&cmd));

    // Wait on all the async processes to finish
    if (!procs_wait_and_reset(&procs)) return 1;

    // TODO: add some examples with nob_procs_append_with_flush()

    return 0;
}

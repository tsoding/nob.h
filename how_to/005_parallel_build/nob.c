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
    
    // --- First build batch: run 3 commands in parallel ---
    // Spawn three async processes collecting them to procs dynamic array
    cmd_append(&cmd, "cc", "-o", BUILD_FOLDER"foo", SRC_FOLDER"foo.c");
    da_append(&procs, cmd_run_async_and_reset(&cmd));
    cmd_append(&cmd, "cc", "-o", BUILD_FOLDER"bar", SRC_FOLDER"bar.c");
    da_append(&procs, cmd_run_async_and_reset(&cmd));
    cmd_append(&cmd, "cc", "-o", BUILD_FOLDER"baz", SRC_FOLDER"baz.c");
    da_append(&procs, cmd_run_async_and_reset(&cmd));

    // Wait on all the async processes to finish
    if (!procs_wait_and_reset(&procs)) return 1;

     // --- Second build batch using nob_procs_append_with_flush ---
    // This allows limiting how many parallel jobs run at once

    const char *files[] = {"alpha.c", "beta.c", "gamma.c"};
    const size_t max_parallel_jobs = 2;

    for (size_t i = 0; i < NOB_ARRAY_LEN(files); ++i) {
        cmd_append(&cmd, "cc", "-o");
        cmd_append(&cmd, BUILD_FOLDER);
        cmd_append(&cmd, nob_temp_sprintf("%.*s", NOB_PATH_REMOVE_EXTENSION(files[i])), NULL);
        cmd_append(&cmd, SRC_FOLDER);
        cmd_append(&cmd, files[i]);

        // Run the build command asynchronously
        Proc proc = cmd_run_async_and_reset(&cmd);

        // Append the process to the list and flush (wait) if max is reached
        if (!procs_append_with_flush(&procs, proc, max_parallel_jobs)) return 1;
    }

    // Wait for any remaining background builds to finish
    if (!procs_wait_and_reset(&procs)) return 1;

    return 0;
}

#define NOB_IMPLEMENTATION
#include "nob.h"

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF_PLUS(argc, argv, "nob.h");
    if (!nob_mkdir_if_not_exists("build")) return 1;

    Nob_Cmd cmd = {0};
    // Cache data will be read and written to a .file_path field ("build/nob.cache" in this case.)
    // In case .file_path field is not set, cache data will only be in memory.
    Nob_Strace_Cache cache = { .file_path = "build/nob.cache" };

    for (int i = 0; i < 10; ++i) {
        nob_cc(&cmd);
        nob_cc_flags(&cmd);
        nob_cc_output(&cmd, "build/foo");
        nob_cc_inputs(&cmd, "src/foo.c");
        if (!nob_cmd_run(&cmd, .strace_cache = &cache)) return 1;

        // Don't cache this command.
        nob_cmd_append(&cmd, "./build/foo");
        if (!nob_cmd_run(&cmd)) return 1;
    }

    // Save cache data to disc.
    // And free all memory malloced by strace_cache.
    nob_strace_cache_finish(cache);

    nob_cmd_free(cmd);

    return 0;
}

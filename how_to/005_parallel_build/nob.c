#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_WARN_DEPRECATED
#include "nob.h"

#define BUILD_FOLDER "build/"
#define SRC_FOLDER "src/"

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    Cmd cmd = {0};
    Procs procs = {0};

    if (!mkdir_if_not_exists(BUILD_FOLDER)) return 1;

    static struct {
        const char *bin_path;
        const char *src_path;
    } targets[] = {
        { .bin_path = BUILD_FOLDER"foo", .src_path = SRC_FOLDER"foo.c" },
        { .bin_path = BUILD_FOLDER"bar", .src_path = SRC_FOLDER"bar.c" },
        { .bin_path = BUILD_FOLDER"baz", .src_path = SRC_FOLDER"baz.c" },
    };

    // Spawn one async process per target collecting them to procs dynamic array
    for (size_t i = 0; i < ARRAY_LEN(targets); ++i) {
        nob_cc(&cmd);
        nob_cc_flags(&cmd);
        nob_cc_output(&cmd, targets[i].bin_path);
        nob_cc_inputs(&cmd, targets[i].src_path);
        if (!cmd_run(&cmd, .async = &procs)) return 1;
    }

    // Wait on all the async processes to finish and reset procs dynamic array to 0
    if (!procs_flush(&procs)) return 1;

    // TODO: add some examples with .max_procs

    return 0;
}

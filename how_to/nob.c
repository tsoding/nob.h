// Makes sure the examples are buildable. Used usually on CI.
#include "../shared.h"
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "../nob.h"

const char *examples[] = {
    "001_basic_usage",
    "005_parallel_build",
    "010_nob_two_stage",
};

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    Cmd cmd = {0};

    for (size_t i = 0; i < ARRAY_LEN(examples); ++i) {
        const char *example = examples[i];
        nob_log(INFO, "--- %s ---", example);
        size_t mark = temp_save();
            if (!set_current_dir(temp_sprintf("./%s", example))) return 1;
                nob_cc(&cmd);
                nob_cc_output(&cmd, "./nob");
                nob_cc_inputs(&cmd, "nob.c");
                if (!cmd_run_sync_and_reset(&cmd)) return 1;

                cmd_append(&cmd, "./nob");
                if (!cmd_run_sync_and_reset(&cmd)) return 1;
            if (!set_current_dir("..")) return 1;
        temp_rewind(mark);
    }

    return 0;
}

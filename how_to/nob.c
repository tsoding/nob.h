// Make sure all the examples are buildable
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "../nob.h"
#include "../shared.h"

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    Cmd cmd = {0};

    const char *parent = ".";
    File_Paths children = {0};
    if (!read_entire_dir(parent, &children)) return 1;

    da_foreach(const char *, child, &children) {
        if (**child == '.') continue;
        if (get_file_type(*child) == FILE_DIRECTORY) {
            nob_log(INFO, "--- %s ---", *child);
            size_t mark = temp_save();
                if (!set_current_dir(temp_sprintf("%s/%s", parent, *child))) return 1;
                if (!build_exec(&cmd, "nob", "nob.c")) return 1;
                cmd_append(&cmd, "./nob");
                if (!cmd_run_sync_and_reset(&cmd)) return 1;
                if (!set_current_dir("..")) return 1;
            temp_rewind(mark);
        }
    }

    return 0;
}

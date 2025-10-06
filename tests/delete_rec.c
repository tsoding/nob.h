#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

int main(void) {
    Cmd cmd = {0};
    bool result;

    mkdir_if_not_exists("dir");
    cmd_append(&cmd, "touch", "dir/file1", "dir/file2", "dir/file3");
    if (!cmd_run(&cmd)) return 1;
    mkdir_if_not_exists("dir/subdir1");
    mkdir_if_not_exists("dir/subdir2");
    mkdir_if_not_exists("dir/subdir3");
    cmd_append(&cmd, "touch", "dir/subdir1/file", "dir/subdir2/file", "dir/subdir3/file");
    if (!cmd_run(&cmd)) return 1;

    result = !delete_directory_recursively("./dir");

    return result;
}

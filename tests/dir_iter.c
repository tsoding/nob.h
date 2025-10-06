#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

int main(void) {
    Dir_Iter iter = {0};
    if (!dir_iter_open(&iter, "."))
        return 1;

    Cmd cmd = {0};
    cmd_append(&cmd, "touch", "file1", "file2", "file3");
    if (!cmd_run(&cmd)) return 1;

    while (dir_iter_next(&iter)) {
        const char *dname = dir_iter_getname(iter);
        if (dname == NULL)
            continue;
        nob_log(NOB_INFO, "dname: %s", dname);
    }

    dir_iter_close(iter);

    return 0;
}

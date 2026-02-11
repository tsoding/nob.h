#define NOB_IMPLEMENTATION
#include "nob.h"

bool count_files(Nob_Walk_Entry entry)
{
    size_t *count = entry.data;
    nob_log(INFO, "%*s%s", (int)entry.level*2, "", entry.path);
    if (entry.type == FILE_REGULAR) *count += 1;
    return true;
}

int main(int argc, char **argv)
{
    GO_REBUILD_URSELF(argc, argv);
    size_t count = 0;
    const char *dir_path = "./data/";
    if (!nob_walk_dir(dir_path, count_files, .data = &count)) return 1;
    nob_log(INFO, "%s contains %zu files", dir_path, count);
    return 0;
}

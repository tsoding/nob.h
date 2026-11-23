#include "shared.h"

static bool count_visit(Walk_Entry entry)
{
    ++*(int *)entry.data;
    return true;
}

int main(void)
{
    int visits = 0;
    if (walk_dir("missing", count_visit, .data = &visits)) return 1;
    printf("visits to missing path: %d\n", visits);
    if (copy_directory_recursively("missing", "dest")) return 1;
    if (delete_file("missing")) return 1;
    return 0;
}

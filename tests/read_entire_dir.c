#include "shared.h"
#define NOB_IMPLEMENTATION
#include "nob.h"

int compar_paths(const void *a, const void *b)
{
    const char* const* ap = (const char* const*)a;
    const char* const* bp = (const char* const*)b;
    return strcmp(*ap, *bp);
}

int main(void)
{
    if (!write_entire_file("foo.txt", NULL, 0)) return 1;
    if (!write_entire_file("bar.txt", NULL, 0)) return 1;
    if (!write_entire_file("baz.txt", NULL, 0)) return 1;

    Nob_File_Paths children = {0};
    if (!nob_read_entire_dir(".", &children)) return 1;
    qsort(children.items, children.count, sizeof(*children.items), compar_paths);
    nob_log(INFO, "Tests:");
    for (size_t i = 0; i < children.count; ++i) {
        if (*children.items[i] == '.') continue;
        nob_log(INFO, "    %s", children.items[i]);
    }
    return 0;
}

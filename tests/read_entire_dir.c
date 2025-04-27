#include "shared.h"
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

int main(void)
{
    if (!write_entire_file("foo.txt", NULL, 0)) return 1;
    if (!write_entire_file("bar.txt", NULL, 0)) return 1;
    if (!write_entire_file("baz.txt", NULL, 0)) return 1;

    Nob_File_Paths children = {0};
    if (!nob_read_entire_dir(".", &children)) return 1;
    nob_log(INFO, "Tests:");
    for (size_t i = 0; i < children.count; ++i) {
        if (*children.items[i] != '.') {
            nob_log(INFO, "    %s", children.items[i]);
        }
    }
    return 0;
}

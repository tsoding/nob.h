#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"
#include "shared.h"

int main(void)
{
    Nob_File_Paths children = {0};
    if (!nob_read_entire_dir(TESTS_FOLDER, &children)) return 1;
    nob_log(INFO, "Tests:");
    for (size_t i = 0; i < children.count; ++i) {
        if (*children.items[i] != '.') {
            nob_log(INFO, "    %s", children.items[i]);
        }
    }
    return 0;
}

#include "shared.h"

int main(void)
{
    if (!mkdir_if_not_exists("dir")) return 1;
    if (!mkdir_if_not_exists("dir/")) return 1;
    if (!write_entire_file("file", NULL, 0)) return 1;
    printf("mkdir over file: %s\n", mkdir_if_not_exists("file") ? "success" : "failure");
#ifndef _WIN32
    if (symlink("dir", "dir-link") == 0 && !mkdir_if_not_exists("dir-link")) return 1;
#endif
    return 0;
}

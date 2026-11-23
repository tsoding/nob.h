#include "shared.h"

int main(void)
{
    if (!write_entire_file("out", "abc", 3)) return 1;
    if (!write_entire_file("out", NULL, 0)) return 1;
    FILE *f = fopen("out", "rb");
    if (f == NULL) return 1;
    bool empty = fgetc(f) == EOF && feof(f);
    fclose(f);
    printf("empty file: %s\n", empty ? "yes" : "no");
#ifdef __linux__
    if (file_exists("/dev/full") && write_entire_file("/dev/full", "x", 1)) return 1;
#endif
    return 0;
}

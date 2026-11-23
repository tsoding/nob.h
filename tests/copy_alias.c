#include "shared.h"

int main(void)
{
    const char bytes[] = {'a', '\0', 'b'};
    if (!write_entire_file("source", bytes, sizeof(bytes))) return 1;
#ifndef _WIN32
    const char *alias = link("source", "alias") == 0 ? "alias" : "./source";
    if (copy_file("source", alias)) return 1;
#endif
    if (!write_entire_file("dest", "longer", 6)) return 1;
    if (!copy_file("source", "dest")) return 1;
    String_Builder sb = {0};
    if (!read_entire_file("dest", &sb)) return 1;
    if (sb.count != sizeof(bytes) || memcmp(sb.items, bytes, sb.count) != 0) return 1;
    printf("Copied %zu bytes\n", sb.count);
    sb_free(sb);
    return 0;
}

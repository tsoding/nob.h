#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_DA_INIT_CAP 4
#include "nob.h"

int main(void)
{
    String_Builder sb = {0};
    nob_log(INFO, "count = %zu, capacity = %zu, items = \"%.*s\"", sb.count, sb.capacity, (int)sb.count, sb.items);
    sb_appendf(&sb, "Hello, %d.", 69);
    nob_log(INFO, "count = %zu, capacity = %zu, items = \"%.*s\"", sb.count, sb.capacity, (int)sb.count, sb.items);
    return 0;
}

#define NOB_DA_INIT_CAP 4
#include "shared.h"

int main(void)
{
    String_Builder sb = {0};
    printf("count = %zu, capacity = %zu, items = \"%.*s\"\n", sb.count, sb.capacity, (int)sb.count, sb.items);
    sb_appendf(&sb, "Hello, %d.", 69);
    printf("count = %zu, capacity = %zu, items = \"%.*s\"\n", sb.count, sb.capacity, (int)sb.count, sb.items);
    return 0;
}

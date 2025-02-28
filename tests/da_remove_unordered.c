#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

typedef struct {
    int *items;
    size_t count;
    size_t capacity;
} Numbers;

void render_numbers(Numbers xs, String_Builder *sb)
{
    for (size_t i = 0; i < xs.count; ++i) {
        if (i > 0) sb_append_cstr(sb, ", ");
        sb_append_cstr(sb, temp_sprintf("%d", xs.items[i]));
    }
    sb_append_null(sb);
}

int main(void)
{
    String_Builder sb = {0};
    Numbers xs = {0};
    for (int i = 12; i <= 15; ++i) da_append(&xs, i);
    while (xs.count > 0) {
        sb.count = 0;
        render_numbers(xs, &sb);
        nob_log(INFO, "%s", sb.items);
        da_remove_unordered(&xs, 0);
    }
    return 0;
}

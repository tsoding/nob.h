#include "shared.h"

size_t error_counter = 0;

void error_counting_log_handler(Nob_Log_Level level, const char *fmt, va_list args)
{
    UNUSED(level);
    UNUSED(fmt);
    UNUSED(args);
    if (level == ERROR) error_counter++;
}

int compar_paths(const void *a, const void *b)
{
    const char* const* ap = (const char* const*)a;
    const char* const* bp = (const char* const*)b;
    return strcmp(*ap, *bp);
}

int main(void)
{
    Nob_File_Paths empty = {0};

    error_counter = 0;
    nob_log_handler *saved_log_hander = get_log_handler();
    set_log_handler(error_counting_log_handler);
    {
        bool ok = nob_read_entire_dir("", &empty);
        printf("Reading empty dir: ok=%s, error_counter=%zu\n", ok ? "true" : "false", error_counter);
    }
    set_log_handler(saved_log_hander);

    if (!write_entire_file("foo.txt", NULL, 0)) return 1;
    if (!write_entire_file("bar.txt", NULL, 0)) return 1;
    if (!write_entire_file("baz.txt", NULL, 0)) return 1;

    Nob_File_Paths children = {0};
    if (!nob_read_entire_dir(".", &children)) return 1;
    qsort(children.items, children.count, sizeof(*children.items), compar_paths);
    printf("Tests:\n");
    for (size_t i = 0; i < children.count; ++i) {
        if (*children.items[i] == '.') continue;
        printf("    %s\n", children.items[i]);
    }
    return 0;
}

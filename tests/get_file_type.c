#include "shared.h"

size_t error_counter = 0;

void error_counting_log_handler(Nob_Log_Level level, const char *fmt, va_list args)
{
    UNUSED(fmt);
    UNUSED(args);
    if (level == ERROR) error_counter++;
}

bool walk_noop(Nob_Walk_Entry entry)
{
    UNUSED(entry);
    return true;
}

int main(void)
{
    const char *missing = "definitely-does-not-exist-42";

    Nob_Log_Handler *saved_log_handler = get_log_handler();
    set_log_handler(error_counting_log_handler);

    error_counter = 0;
    File_Type type = get_file_type(missing);
    printf("get_file_type(missing): is_error=%s, error_counter=%zu\n",
           type == (File_Type)-1 ? "true" : "false", error_counter);

    error_counter = 0;
    bool walk_ok = walk_dir(missing, walk_noop);
    printf("walk_dir(missing): ok=%s, error_counter=%zu\n",
           walk_ok ? "true" : "false", error_counter);

    error_counter = 0;
    bool copy_ok = copy_directory_recursively(missing, "some-dst");
    printf("copy_directory_recursively(missing): ok=%s, error_counter=%zu\n",
           copy_ok ? "true" : "false", error_counter);

    set_log_handler(saved_log_handler);
    return 0;
}

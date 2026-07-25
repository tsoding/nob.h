#include "shared.h"

size_t error_counter = 0;

void error_counting_log_handler(Nob_Log_Level level, const char *fmt, va_list args)
{
    UNUSED(fmt);
    UNUSED(args);
    if (level == ERROR) error_counter++;
}

int main(void)
{
    String_Builder sb = {0};

    const char *payload = "first line\nsecond line\n";
    if (!write_entire_file("file.txt", payload, strlen(payload))) return 1;
    if (!read_entire_file("file.txt", &sb)) return 1;
    printf("regular file: read %zu bytes\n", sb.count);
    printf("%.*s", (int)sb.count, sb.items);

    if (!read_entire_file("file.txt", &sb)) return 1;
    printf("read again: %zu bytes total\n", sb.count);

    Nob_Log_Handler *saved_log_handler = get_log_handler();
    set_log_handler(error_counting_log_handler);
    bool ok = read_entire_file("does-not-exist.txt", &sb);
    set_log_handler(saved_log_handler);
    printf("missing file: ok=%s, error_counter=%zu, count=%zu\n", ok ? "true" : "false", error_counter, sb.count);

#ifndef _WIN32
    if (mkfifo("fifo", 0644) < 0) return 1;
    pid_t pid = fork();
    if (pid < 0) return 1;
    if (pid == 0) {
        FILE *f = fopen("fifo", "wb");
        if (f == NULL) _exit(1);
        fputs("hello through a fifo\n", f);
        fclose(f);
        _exit(0);
    }
    sb.count = 0;
    if (!read_entire_file("fifo", &sb)) return 1;
    if (waitpid(pid, NULL, 0) < 0) return 1;
    printf("fifo: read %zu bytes\n", sb.count);
    printf("%.*s", (int)sb.count, sb.items);
#endif

    return 0;
}

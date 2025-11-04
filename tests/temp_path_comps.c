#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

int main(void)
{
    static const char *paths[] = {
#ifdef _WIN32
        "C:",
        "C:\\Program Files\\Aboba",
        "C:\\Program Files\\Aboba\\File.exe",
#else
        "/",
        "/home/rexim",
        "/home/rexim/",
        "/home/rexim////",
        "/home/rexim/file.txt",
#endif
        "",
        ".",
        NULL,
    };

    nob_log(INFO, "temp_dir_name:");
    for (size_t i = 0; i < ARRAY_LEN(paths); ++i) {
        nob_log(INFO, "    %s => %s", paths[i], temp_dir_name(paths[i]));
    }
    nob_log(INFO, "temp_file_name:");
    for (size_t i = 0; i < ARRAY_LEN(paths); ++i) {
        nob_log(INFO, "    %s => %s", paths[i], temp_file_name(paths[i]));
    }
    nob_log(INFO, "temp_file_ext:");
    for (size_t i = 0; i < ARRAY_LEN(paths); ++i) {
        nob_log(INFO, "    %s => %s", paths[i], temp_file_ext(paths[i]));
    }
    return 0;
}

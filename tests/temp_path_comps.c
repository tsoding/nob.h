#include "shared.h"

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

    printf("temp_dir_name:\n");
    for (size_t i = 0; i < ARRAY_LEN(paths); ++i) {
        printf("    %s => %s\n", paths[i], temp_dir_name(paths[i]));
    }
    printf("temp_file_name:\n");
    for (size_t i = 0; i < ARRAY_LEN(paths); ++i) {
        printf("    %s => %s\n", paths[i], temp_file_name(paths[i]));
    }
    printf("temp_file_ext:\n");
    for (size_t i = 0; i < ARRAY_LEN(paths); ++i) {
        printf("    %s => %s\n", paths[i], temp_file_ext(paths[i]));
    }
    return 0;
}

#define NOB_IMPLEMENTATION
#include "nob.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    const char *dst = "flag_h";
    const char *repo_url = "https://github.com/tsoding/flag.h";
    
    if (!nob_git_fetch(repo_url, dst, NULL, true)) {
        nob_log(NOB_ERROR, "nob_git_fetch failed");
        return 1;
    }
    
    srand(time(NULL) );
    int expected_number = rand();
    char expected_str[32];
    snprintf(expected_str, sizeof (expected_str), "%d", expected_number);

    char main_c[256];
    int main_c_len = snprintf(main_c, sizeof (main_c),
        "#include <stdio.h>\n"
        "#include \"flag.h\"\n"
        "int main(void){ (void)printf(\"%s\"); return 0; }\n",
        expected_str);

    if (!nob_write_entire_file("git_fetch_main.c", main_c, (size_t)main_c_len)) {
        nob_log(NOB_ERROR, "Could not write git_fetch_main.c");
        return 1;
    }

    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, "cc", "-I", dst, "-o", "./git_fetch_main", "git_fetch_main.c");
    if (!nob_cmd_run(&cmd)) {
        nob_log(NOB_ERROR, "compile failed");
        return 1;
    }

    nob_cmd_append(&cmd, "./git_fetch_main");
    if (!nob_cmd_run(&cmd, .stdout_path = "./git_fetch_main_out.txt")) {
        nob_log(NOB_ERROR, "run failed");
        return 1;
    }

    Nob_String_Builder sb = {0};

    if (!nob_read_entire_file("./git_fetch_main_out.txt", &sb)) return 1;

    Nob_String_View out = nob_sb_to_sv(sb);

    Nob_String_View trimmed = nob_sv_trim(out);
    if (!nob_sv_eq(trimmed, nob_sv_from_cstr(expected_str))) {
        nob_log(NOB_ERROR, "Unexpected output (see file)");
        return 1;
    }

    nob_log(NOB_INFO, "git_fetch_main test passed");
    return 0;
}

#include <string.h>
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

int main(void) {
    Cmd cmd = {0};
    Procs procs = {0};

    size_t max_procs_count = 4;

    static const char *job_content = 
        "#include <stdio.h>" "\n"
        "int main(int argc, char **argv) {" "\n"
        "    if (argc != 2) return 1;" "\n"
        "    char *job_id = argv[1];" "\n"
        "    printf(\"Job %*s start\\n\", 3, job_id);" "\n"
        "    for (volatile long long i=0; i < 10ll * 1000ll * 1000ll; ++i) ; " "\n"
        "    printf(\"Job %*s done \\n\", 3, job_id);" "\n"
        "    return 0; " "\n"
        "}";
    if (!nob_write_entire_file("job.c", job_content, strlen(job_content))) return 1;

    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    nob_cc_inputs(&cmd, "job.c");
    nob_cc_output(&cmd, "./job");
    if (!cmd_run_sync_and_reset(&cmd)) return 1;

    for (int i = 0; i < 20; ++i) {
        cmd_append(&cmd, "./job", temp_sprintf("%d", i));
        if (!procs_append_with_wait_any(&procs, cmd_run_async_and_reset(&cmd), max_procs_count)) return 1;
    }
    if (!procs_wait_and_reset(&procs)) return 1;

    return 0;
}

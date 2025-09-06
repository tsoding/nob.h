#define NOB_IMPLEMENTATION
#include "nob.h"

// Just for debugging and to see what is cached in human readable format
static void nob__ptrace_cache_print_makefile(Nob_Ptrace_Cache cache);
static void nob__ptrace_cache_print_debug(Nob_Ptrace_Cache cache);

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF_PLUS(argc, argv, "nob.h");
    if (!nob_mkdir_if_not_exists("build")) return 1;

    Nob_Cmd cmd = {0};
    // Cache data will be read and written to a .file_path field ("build/nob.cache" in this case.)
    // In case .file_path field is not set, cache data will only be in memory.
    Nob_Ptrace_Cache cache = { .file_path = "build/nob.cache" };

    for (int i = 0; i < 10; ++i) {
        nob_cc(&cmd);
        nob_cc_flags(&cmd);
        nob_cc_output(&cmd, "build/foo");
        nob_cc_inputs(&cmd, "src/foo.c");
        if (!nob_cmd_run(&cmd, .ptrace_cache = &cache)) return 1;

        // Don't cache this command.
        nob_cmd_append(&cmd, "./build/foo");
        if (!nob_cmd_run(&cmd)) return 1;
    }

    // Convert Nob_Ptrace_Cache to Makefile and print it
    nob__ptrace_cache_print_makefile(cache);
    nob__ptrace_cache_print_debug(cache);

    // Save cache data to disc.
    // And free all memory malloced by ptrace_cache.
    nob_ptrace_cache_finish(cache);

    nob_cmd_free(cmd);

    return 0;
}

#if NOB_HAS_PTRACE_CACHE
static void nob__ptrace_cache_print_makefile_rec(Nob_Ptrace_Cache cache, Nob_String_Builder* cur_command, int parent)
{
    size_t old_count = cur_command->count;
    for (size_t i = 0; i < cache.nodes.count; ++i) {
        Nob__Ptrace_Cache_Node node = cache.nodes.items[i];
        if (node.parent != parent) continue;
        if (node.kind == Nob__Ptrace_Cache_Node_Arg) nob_sb_append_cstr(cur_command, " ");
        if (node.kind == Nob__Ptrace_Cache_Node_Stdin) nob_sb_append_cstr(cur_command, " < ");
        if (node.kind == Nob__Ptrace_Cache_Node_Stdout) nob_sb_append_cstr(cur_command, " > ");
        if (node.kind == Nob__Ptrace_Cache_Node_Stderr) nob_sb_append_cstr(cur_command, " 2> ");
        nob_sb_append_cstr(cur_command, &cache.arena.items[node.arg_index]);
        if (node.input_paths.count != 0 || node.output_paths.count != 0) {
            for (size_t j = 0; j < node.output_paths.count;) {
                printf("%s", &cache.arena.items[node.output_paths.items[j]]);
                if (++j < node.output_paths.count) printf(" "); 
            }
            printf(": ");
            for (size_t j = 0; j < node.input_paths.count; ++j) printf("%s ", &cache.arena.items[node.input_paths.items[j]]);
            printf("\n\t");
            printf("%.*s\n\n", (int)cur_command->count, cur_command->items);
        }
        nob__ptrace_cache_print_makefile_rec(cache, cur_command, (int)i);
        cur_command->count = old_count;
    }
}

static void nob__ptrace_cache_print_makefile(Nob_Ptrace_Cache cache)
{
    Nob_String_Builder cur_command = { 0 };
    nob__ptrace_cache_print_makefile_rec(cache, &cur_command, -1);
    nob_sb_free(cur_command);
}

static void nob__ptrace_cache_print_debug(Nob_Ptrace_Cache cache)
{
    printf("-----\n");
    for (size_t i = 0; i < cache.nodes.count; ++i) {
        Nob__Ptrace_Cache_Node node = cache.nodes.items[i];
        printf("[%zu]%s(parent=%d, kind=%d, in_c=%zu, out_c=%zu)\n", i, &cache.arena.items[node.arg_index], node.parent, node.kind, node.input_paths.count, node.output_paths.count);
    }
    printf("-----\n");
}

#else
static void nob__ptrace_cache_print_makefile(Nob_Ptrace_Cache cache)
{
    printf("Ptrace not implmented on this platform\n");
}

static void nob__ptrace_cache_print_debug(Nob_Ptrace_Cache cache)
{
    printf("Ptrace not implmented on this platform\n");
}
#endif // NOB_HAS_PTRACE_CACHE

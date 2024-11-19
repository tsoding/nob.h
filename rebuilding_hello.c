#define NOB_IMPLEMENTATION
#include "nob.h"

void dump_nop_cmd_ptr(Nob_Cmd* opts)
{
    printf("opts at %p \n", (void*)opts);
    printf("opts: %ld", opts->count);
    for (size_t i=0; i<opts->count; i++)
        printf(", %s ", opts->items[i] );
    putchar('\n');
}

typedef struct  {
    int      argc;
    char**   argv;
    Nob_Cmd* opts;
} args_stack;

void dump_build_args(const char *source_path, args_stack args)
{
    Nob_Cmd* opts = args.opts;
    const char *binary_path = nob_shift(args.argv, args.argc);
    printf("%s %s \n", source_path, binary_path);
    if (opts) dump_nop_cmd_ptr(opts);
} 

#define DO_REBUILD(...) dump_build_args(__FILE__, (args_stack){__VA_ARGS__})


int main(int argc, char** argv) 
{
    Nob_Cmd options = {0};
    nob_cmd_append(&options, "-Wall", "-Wextra", "-I../"); 
    NOB_GO_REBUILD_URSELF_WITH_OPTIONS(argc, argv, &options);
    //nob_cmd_free(options);

    printf("Hello JIT\n");

    // see if we can hide this extension in a MACRO ...
    // well it does but with compilation Warnings ...
    // so ... nob soo nice !
    printf("\n << with No options >>\n");
    DO_REBUILD(argc, argv);
    printf("\n <<  with  options  >>\n");
    DO_REBUILD(argc, argv, &options);
    
    return 0;
}

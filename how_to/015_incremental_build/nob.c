/*
Incremental build example.
main requires: main.c foo.so bar.c bar.h
foo.so requires: foo.c foo.h baz.h
-----------------------------------------------------
You are invited to play around with the sources and see when everything is rebuilt and when only
the main is rebuilt and relinked.
*/
#define NOB_IMPLEMENTATION
#define NOB_EXPERIMENTAL_DELETE_OLD // Trying this experimental feature
#include "nob.h"

#define BUILD_FOLDER "build/"
#define SRC_FOLDER   "src/"

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF_PLUS(argc, argv, "../../nob.h");
    const char *program = nob_shift_args(&argc, &argv);
    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;

    // create dependency of the sources
    Nob_Dependency main_c = nob_create_dependency(SRC_FOLDER"main.c");
    Nob_Dependency foo_c  = nob_create_dependency(SRC_FOLDER"foo.c");
    Nob_Dependency foo_h  = nob_create_dependency(SRC_FOLDER"foo.h");
    Nob_Dependency baz_h  = nob_create_dependency(SRC_FOLDER"baz.h");
    Nob_Dependency bar_c  = nob_create_dependency(SRC_FOLDER"bar.c");
    Nob_Dependency bar_h  = nob_create_dependency(SRC_FOLDER"bar.h");

    // foo_so
    Nob_Dependency foo_so = nob_create_dependency(BUILD_FOLDER"foo.so");
    nob_preq_append(foo_so.preq, foo_c, foo_h, baz_h);
    nob_cc(foo_so.cmd);
    nob_cc_flags(foo_so.cmd);
    nob_cc_output(foo_so.cmd, foo_so.target);
    nob_cc_inputs(foo_so.cmd, foo_c.target, foo_h.target, baz_h.target);
    nob_cc_inputs(foo_so.cmd, "-fPIC", "-shared");

    // bin
    Nob_Dependency bin = nob_create_dependency(BUILD_FOLDER"main");
    nob_preq_append(bin.preq, main_c, foo_so, bar_c, bar_h);

    nob_cc(bin.cmd);
    nob_cc_flags (bin.cmd);
    nob_cc_output(bin.cmd, bin.target);
    nob_cc_inputs(bin.cmd, main_c.target, bar_c.target, bar_h.target);
    nob_cc_inputs(bin.cmd, "-L"BUILD_FOLDER, foo_so.target);

    if (!nob_incremental_build(bin)) return 1;

    if (argc > 0) {
        const char *subcmd = nob_shift_args(&argc, &argv);
        if (strcmp(subcmd, "run") == 0) {
            Nob_Cmd cmd = {0};
            nob_cmd_append(&cmd, BUILD_FOLDER"main");
            if (!nob_cmd_run_sync(cmd)) return 1;
            NOB_FREE(cmd.items);
        }
    }

    NOB_FREE_DEPENDENCY(foo_so);
    NOB_FREE_DEPENDENCY(bin);

    return 0;
}

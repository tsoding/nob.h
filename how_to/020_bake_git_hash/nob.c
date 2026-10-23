#define NOB_IMPLEMENTATION
#include "nob.h"

#define BUILD_FOLDER "build/"
#define SRC_FOLDER   "src/"

bool generate_build_h(void)

int main(int argc, char **argv)
{
    GO_REBUILD_URSELF(argc, argv);

    if (!mkdir_if_not_exists(BUILD_FOLDER)) return 1;

    // Generating "build.h" which is used by src/main.c.
    if (!generate_build_h()) return 1;

    Cmd cmd = {0};
    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    cmd_append(&cmd, "-I"BUILD_FOLDER);
    nob_cc_output(&cmd, BUILD_FOLDER"main");
    nob_cc_inputs(&cmd, SRC_FOLDER"main.c");
    if (!cmd_run(&cmd)) return 1;

    cmd_append(&cmd, BUILD_FOLDER"main");
    if (!cmd_run(&cmd)) return 1;

    return 0;
}

bool generate_build_h(void)
{
    bool result = true;
    String_Builder build_h = {0};
    String_Builder git_hash = {0};

    sb_appendf(&build_h, "#pragma once\n");

    // sb_append_git_hash is provided by nob.h and it calls a couple of git commands
    // to acquire the current git hash of the repo.
    switch (sb_append_git_hash(&git_hash, BUILD_FOLDER)) {
    case GIT_HASH_OK:
        sb_appendf  (&build_h, "#define GIT_HASH \"");
        sb_append_sb(&build_h, git_hash);
        sb_appendf  (&build_h, "\"\n");
        break;
    case GIT_HASH_NO_GIT:
        // If you are building outside of git repo, do not fail the build!
        // Simply do not generate GIT_HASH. The user might've acquired the
        // source code not through git-clone, and it's nice to keep the project
        // still buildable. Later in the main program you can easily #ifdef
        // the GIT_HASH and just display a special message that it is not available.
        nob_log(WARNING, "Git command failed! Git hash will not be included in the build.");
        break;
    case GIT_HASH_ERROR:
        return_defer(false);
    default:
        UNREACHABLE("Git_Hash_Result");
    }

    if (!write_entire_file(BUILD_FOLDER"build.h", build_h.items, build_h.count)) return_defer(false);

defer:
    free(build_h.items);
    free(git_hash.items);
    return result;
}

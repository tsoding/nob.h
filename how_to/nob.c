// Makes sure the examples are buildable. Used usually on CI.
#include "../shared.h"

bool visit(Nob_Walk_Entry entry)
{
    if (entry.type == FILE_DIRECTORY && entry.level == 1) {
        Cmd *cmd = (Cmd*)entry.data;
        size_t mark = temp_save();
            if (!set_current_dir(entry.path)) return 1;
                nob_cc(cmd);
                nob_cc_flags(cmd);
                nob_cc_output(cmd, "./nob");
                nob_cc_inputs(cmd, "nob.c");
                if (!cmd_run(cmd)) return 1;

                cmd_append(cmd, "./nob");
                if (!cmd_run(cmd)) return 1;
            if (!set_current_dir("..")) return 1;
        temp_rewind(mark);

        *entry.action = WALK_SKIP;
    }
    return true;
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    Cmd cmd = {0};
    if (!nob_walk_dir(".", visit, .data = &cmd)) return 1;

    return 0;
}

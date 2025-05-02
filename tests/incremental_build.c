#define NOB_STRIP_PREFIX
#define NOB_IMPLEMENTATION
#include "nob.h"

int total = 0;
int pass  = 0;
int fail  = 0;

int test_circular_dependency()
{
    total += 1;

    Dependency foo_c = create_dependency("./foo.c");
    Dependency bar_c = create_dependency("./bar.c");
    Dependency bin_c = create_dependency("./bin.c");

    Dependency foo_o = create_dependency("./foo.o");
    Dependency bar_o = create_dependency("./bar.o");
    Dependency bin   = create_dependency("./bin");

    preq_append(&foo_o.preq, &foo_c, &bar_o);
    preq_append(&bar_o.preq, &bar_c, &foo_o);
    preq_append(&bin.preq  , &bar_o, &foo_o);

    cmd_append(&foo_o.cmd, "gcc", "-c", foo_c.target, bar_o.target); // circular dependency
    cmd_append(&bar_o.cmd, "gcc", "-c", bar_c.target, foo_o.target); // circular dependency
    cmd_append(&bin.cmd  , "gcc", "-o", bin.target, foo_o.target, bar_o.target);

    const char *src = "int main() {return 0;}";
    if (!nob_write_entire_file(bin_c.target, src, strlen(src))) return 1;
    if (!nob_write_entire_file(foo_c.target, "", strlen(""))) return 1;
    if (!nob_write_entire_file(bar_c.target, "", strlen(""))) return 1;

    nob_log(NOB_WARNING, "This test should fail.");

    if (!incremental_build(bin)) {
        pass += 1;
        nob_log(NOB_INFO, "test_circular_dependency ... OK");
        return 0;
    } else {
        fail += 1;
        nob_log(NOB_INFO, "test_circular_dependency ... FAIL");
        return 1;
    }
}

int main(void)
{
    test_circular_dependency();
    nob_log(NOB_INFO, "Incremental build tests: %d total, %d passed, %d failed.", total, pass, fail);
    return 0;
}

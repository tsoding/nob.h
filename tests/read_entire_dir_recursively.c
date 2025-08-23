#include "shared.h"
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

int main(void)
{
    // build/tests/read_entire_dir_recursively.cwd
    // ├── external
    // │   ├── foobar
    // │   │   ├── foobar.h
    // │   │   ├── libfoobar.a
    // │   │   └── libfoobar.so
    // │   └── foobarbaz
    // │       ├── foobarbaz.h
    // │       ├── libfoobarbaz.a
    // │       └── libfoobarbaz.so
    // ├── include
    // │   ├── bar
    // │   │   └── bar.c
    // │   ├── baz.c
    // │   └── foo
    // │       └── foo.c
    // └── src
    //     ├── bar
    //     │   └── bar.c
    //     ├── baz.c
    //     └── foo
    //         └── foo.c
    if (!mkdir_if_not_exists("src")) return 1;
    if (!mkdir_if_not_exists("src/foo")) return 1;
    if (!mkdir_if_not_exists("src/bar")) return 1;
    if (!mkdir_if_not_exists("include")) return 1;
    if (!mkdir_if_not_exists("include/foo")) return 1;
    if (!mkdir_if_not_exists("include/bar")) return 1;
    if (!mkdir_if_not_exists("external")) return 1;
    if (!mkdir_if_not_exists("external/foobar")) return 1;
    if (!mkdir_if_not_exists("external/foobarbaz")) return 1;
    if (!write_entire_file("src/foo/foo.c", NULL, 0)) return 1;
    if (!write_entire_file("src/bar/bar.c", NULL, 0)) return 1;
    if (!write_entire_file("src/baz.c", NULL, 0)) return 1;
    if (!write_entire_file("include/foo/foo.c", NULL, 0)) return 1;
    if (!write_entire_file("include/bar/bar.c", NULL, 0)) return 1;
    if (!write_entire_file("include/baz.c", NULL, 0)) return 1;
    if (!write_entire_file("external/foobar/foobar.h", NULL, 0)) return 1;
    if (!write_entire_file("external/foobar/libfoobar.a", NULL, 0)) return 1;
    if (!write_entire_file("external/foobar/libfoobar.so", NULL, 0)) return 1;
    if (!write_entire_file("external/foobarbaz/foobarbaz.h", NULL, 0)) return 1;
    if (!write_entire_file("external/foobarbaz/libfoobarbaz.a", NULL, 0)) return 1;
    if (!write_entire_file("external/foobarbaz/libfoobarbaz.so", NULL, 0)) return 1;

    File_Paths children = {0};
    if (!read_entire_dir_recursively(".", &children)) return 1;
    nob_log(INFO, "Tests:");
    for (size_t i = 0; i < children.count; ++i) {
        nob_log(INFO, "    %s", children.items[i]);
    }
    return 0;
}

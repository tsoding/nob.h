#include "shared.h"
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

bool mktestdir() {
    // build/tests/read_entire_dir.cwd
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
    // │   │   └── bar.h
    // │   ├── baz.h
    // │   └── foo
    // │       └── foo.h
    // └── src
    //     ├── bar
    //     │   └── bar.c
    //     ├── baz.c
    //     └── foo
    //         └── foo.c
    return mkdir_if_not_exists("src")
        && mkdir_if_not_exists("src/foo")
        && mkdir_if_not_exists("src/bar")
        && mkdir_if_not_exists("include")
        && mkdir_if_not_exists("include/foo")
        && mkdir_if_not_exists("include/bar")
        && mkdir_if_not_exists("external")
        && mkdir_if_not_exists("external/foobar")
        && mkdir_if_not_exists("external/foobarbaz")
        && write_entire_file("src/foo/foo.c", NULL, 0)
        && write_entire_file("src/bar/bar.c", NULL, 0)
        && write_entire_file("src/baz.c", NULL, 0)
        && write_entire_file("include/foo/foo.h", NULL, 0)
        && write_entire_file("include/bar/bar.h", NULL, 0)
        && write_entire_file("include/baz.h", NULL, 0)
        && write_entire_file("external/foobar/foobar.h", NULL, 0)
        && write_entire_file("external/foobar/libfoobar.a", NULL, 0)
        && write_entire_file("external/foobar/libfoobar.so", NULL, 0)
        && write_entire_file("external/foobarbaz/foobarbaz.h", NULL, 0)
        && write_entire_file("external/foobarbaz/libfoobarbaz.a", NULL, 0)
        && write_entire_file("external/foobarbaz/libfoobarbaz.so", NULL, 0);
}

int main(void)
{
    // Test nob_read_entire_dir()
    if (!mktestdir()) return 1;
    Nob_File_Paths children = {0};
    if (!read_entire_dir(".", &children)) return 1;
    nob_log(INFO, "read_entire_dir():");
    da_foreach(const char *, child, &children) {
        if (**child != '.') {
            nob_log(INFO, "    %s", *child);
        }
    }

    // Test nob_read_entire_dir() with recursively option
    nob_da_free(children);
    children = (Nob_File_Paths){0};
    if (!read_entire_dir(".", &children, .recursively = true)) return 1;
    nob_log(INFO, "read_entire_dir() recursively:");
    da_foreach(const char *, child, &children) {
        nob_log(INFO, "    %s", *child);
    }

    // Test nob_read_entire_dir() with wildcard option
    File_Paths srcs = {0}, headers = {0}, libs = {0}, dlls = {0};
    if (!read_entire_dir(".", &srcs, .wildcard = "src/*.c")) return 1;
    if (!read_entire_dir(".", &srcs, .wildcard = "src/**/*.c")) return 1;
    if (!read_entire_dir(".", &headers, .wildcard = "include/*.h")) return 1;
    if (!read_entire_dir(".", &headers, .wildcard = "include/**/*.h")) return 1;
    if (!read_entire_dir(".", &headers, .wildcard = "external/**/*.h")) return 1;
    if (!read_entire_dir(".", &libs, .wildcard = "external/**/*.a")) return 1;
    if (!read_entire_dir(".", &dlls, .wildcard = "external/**/*.so")) return 1;
    nob_log(INFO, "read_entire_dir() wildcard:");
    nob_log(INFO, "srcs:");
    da_foreach(const char *, src, &srcs) {
        nob_log(INFO, "    %s", *src);
    }
    nob_log(INFO, "headers:");
    da_foreach(const char *, header, &headers) {
        nob_log(INFO, "    %s", *header);
    }
    nob_log(INFO, "libs:");
    da_foreach(const char *, lib, &libs) {
        nob_log(INFO, "    %s", *lib);
    }
    nob_log(INFO, "dlls:");
    da_foreach(const char *, dll, &dlls) {
        nob_log(INFO, "    %s", *dll);
    }
    return 0;
}

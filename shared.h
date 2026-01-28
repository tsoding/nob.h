// This is the code that is shared between nob.c, tests/*.c, and how_to/nob.c.
// Not used in the how_to-s themselves as we try to keep them self-contained.
// IMPORTANT! Must be included before including nob.h as it also configures it
#ifndef SHARED_H_
#define SHARED_H_

// Folder must end with forward slash /
#define BUILD_FOLDER "./build/"
#define TESTS_FOLDER "./tests/"

// TODO: we should test on C++ compilers too

#if defined(__cplusplus)
    #if defined(_MSC_VER)
        #define nob_cc_flags(cmd) cmd_append(cmd, "/W4", "/nologo", "/D_CRT_SECURE_NO_WARNINGS", "-I.")
    #else
        #define nob_cc(cmd) cmd_append(cmd, "cc", "-x", "c++")
        #define nob_cc_flags(cmd) cmd_append(cmd, "-Wall", "-Wextra", "-Wno-missing-field-initializers", "-Wswitch-enum", "-ggdb", "-I.");
    #endif
#else // __cplusplus
    #if defined(_MSC_VER)
        #define nob_cc_flags(cmd) cmd_append(cmd, "/W4", "/nologo", "/D_CRT_SECURE_NO_WARNINGS", "-I.")
    #elif defined(__APPLE__) || defined(__MACH__)
        // TODO: "-std=c99", "-D_POSIX_C_SOURCE=200112L" didn't work for MacOS, don't know why, don't really care that much at the moment.
        //   Anybody who does feel free to investigate.
        #define nob_cc_flags(cmd) cmd_append(cmd, "-Wall", "-Wextra", "-Wswitch-enum", "-I.")
    #elif defined(__FreeBSD__)
    // "-D_POSIX_C_SOURCE=200112L" hides required symbols on FreeBSD
        #define nob_cc_flags(cmd) cmd_append(cmd, "-Wall", "-Wextra", "-Wswitch-enum", "-std=c99", "-ggdb", "-I.");
    #else
        #define nob_cc_flags(cmd) cmd_append(cmd, "-Wall", "-Wextra", "-Wswitch-enum", "-std=c99", "-D_POSIX_C_SOURCE=200112L", "-ggdb", "-I.");
    #endif
#endif // __cplusplus

#define NOBDEF static inline
#define NOB_IMPLEMENTATION
#define NOB_EXPERIMENTAL_DELETE_OLD
#define NOB_WARN_DEPRECATED
#define NOB_STRIP_PREFIX        // Testing for backward compatibility after v3.0.0
#include "nob.h"
#undef rename                   // Testing for backward compatibility after v1.20.6

// Utility that tests use to build the tools they need
bool build_tool(Cmd *cmd, Procs *procs, const char *bin_path, const char *src_path, const char *src)
{
    if (!write_entire_file(src_path, src, strlen(src))) return 1;

    nob_cc(cmd);
    nob_cc_flags(cmd);
    nob_cc_output(cmd, bin_path);
    nob_cc_inputs(cmd, src_path);
    return cmd_run(
        cmd,
        .async = procs,
        // To make sure no compiler output pollutes the test output.
        // This is needed specifically for cl.exe cause it has
        // a tendency to output compiled source files to stdout.
        .stdout_path = temp_sprintf("%s.comp.txt", bin_path), 
    );
}

#endif // SHARED_H_

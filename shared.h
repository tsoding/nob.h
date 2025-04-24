// This is the code that is shared between nob.c, tests/*.c, and how_to/nob.c.
// Not used in the how_to-s themselves as we try to keep them self-contained.
// IMPORTANT! Must be included before including nob.h as it also configures it
#ifndef SHARED_H_
#define SHARED_H_

// Folder must end with forward slash /
#define BUILD_FOLDER "build/"
#define TESTS_FOLDER "tests/"

#if defined(_MSC_VER)
#  define nob_cc_flags(cmd) cmd_append(cmd, "-I.")
#elif defined(__APPLE__) || defined(__MACH__)
// TODO: "-std=c99", "-D_POSIX_SOURCE" didn't work for MacOS, don't know why, don't really care that much at the moment.
//   Anybody who does feel free to investigate.
#  define nob_cc_flags(cmd) cmd_append(cmd, "-Wall", "-Wextra", "-Wswitch-enum", "-I.")
#else
#  define nob_cc_flags(cmd) cmd_append(cmd, "-Wall", "-Wextra", "-Wswitch-enum", "-std=c99", "-D_POSIX_SOURCE", "-ggdb", "-I.");
#endif

#endif // SHARED_H_

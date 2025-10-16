// This is your build script. You only need to "bootstrap" it once with `cc -o nob nob.c`
// (you can call the executable whatever actually) or `cl nob.c` on MSVC. After that every
// time you run the `nob` executable if it detects that you modifed nob.c it will rebuild
// itself automatically thanks to NOB_GO_REBUILD_URSELF (see below)

// nob.h is an stb-style library https://github.com/nothings/stb/blob/master/docs/stb_howto.txt
// What that means is that it's a single file that acts both like .c and .h files, but by default
// when you include it, it acts only as .h. To make it include implementations of the functions
// you must define NOB_IMPLEMENTATION macro. This is done to give you full control over where
// the implementations go.
#define NOB_IMPLEMENTATION

// Always keep a copy of nob.h in your repo. One of my main pet peeves with build systems like CMake
// and Autotools is that the codebases that use them naturally rot. That is if you do not actively update
// your build scripts, they may not work with the latest version of the build tools. Here we basically
// include the entirety of the source code of the tool along with the code base. It will never get
// outdated (unless you got no standard compliant C compiler lying around, but at that point why are
// you trying to build a C project?)
//
// (In these examples we actually symlinking nob.h, but this is to keep nob.h-s synced among all the
// examples)
#include "nob.h"

// Some folder paths that we use throughout the build process.
#define BUILD_FOLDER "build/"
#define SRC_FOLDER   "src/"

typedef enum {
    IS_BINARY,
    IS_O_FILE,
} Is;

int go_rebuild_and_reset(const char *binary_path, Is path_is, Nob_File_Paths *dependency_paths);

int main(int argc, char **argv)
{
    // This line enables the self-rebuilding. It detects when nob.c is updated and auto rebuilds it then
    // runs it again.
    NOB_GO_REBUILD_URSELF(argc, argv);

    // It's better to keep all the building artifacts in a separate build folder. Let's create it if it
    // does not exist yet.
    //
    // Majority of the nob command return bool which indicates whether operation has failed or not (true -
    // success, false - failure). If the operation returned false you don't need to log anything, the
    // convention is usually that the function logs what happened to itself. Just do
    // `if (!nob_function()) return;`
    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;
    Nob_File_Paths dependency_paths = {0};

    // For building a `.o` file, the corresponding `.c` file comes first:
    nob_da_append(&dependency_paths, SRC_FOLDER "foo.c");
    // Then any dependencies:
    nob_da_append(&dependency_paths, SRC_FOLDER "foo.h");
    if (!go_rebuild_and_reset(BUILD_FOLDER "foo.o", IS_O_FILE, &dependency_paths)) {
        return 1;
    }

    // For building a `.o` file, the corresponding `.c` file comes first:
    nob_da_append(&dependency_paths, SRC_FOLDER "global.c");
    // Then any dependencies:
    nob_da_append(&dependency_paths, SRC_FOLDER "global.h");
    nob_da_append(&dependency_paths, SRC_FOLDER "foo.h");
    if (!go_rebuild_and_reset(BUILD_FOLDER "global.o", IS_O_FILE, &dependency_paths)) {
        return 1;
    }

    // For building the main executable, just add all `.o` files in.
    nob_da_append(&dependency_paths, SRC_FOLDER "main.c");
    nob_da_append(&dependency_paths, BUILD_FOLDER "foo.o");
    nob_da_append(&dependency_paths, BUILD_FOLDER "global.o");
    if (!go_rebuild_and_reset(BUILD_FOLDER "main", IS_BINARY, &dependency_paths)) {
        return 1;
    }

    return 0;
}

int go_rebuild_and_reset(const char *binary_path, Is path_is, Nob_File_Paths *dependency_paths) {
    int ok = 1;
    int rebuild_is_needed = nob_needs_rebuild(binary_path, dependency_paths->items, dependency_paths->count);
    if (rebuild_is_needed) {
        Nob_Cmd cmd = {0};

        nob_cc(&cmd);
        nob_cc_flags(&cmd);
        nob_cc_output(&cmd, binary_path);
        if (path_is == IS_O_FILE) {
            // Not sure if this is portable, but generate object files only.
            nob_cmd_append(&cmd, "-c");
            // Only the first input is needed for .o files.
            nob_cc_inputs(&cmd, dependency_paths->items[0]);
        } else for (int i = 0; i < dependency_paths->count; ++i) {
            nob_cc_inputs(&cmd, dependency_paths->items[i]);
        }

        if (!nob_cmd_run_sync(cmd)) {
            ok = 0;
        }
    }
    NOB_FREE(dependency_paths->items);
    *dependency_paths = (Nob_File_Paths){0};
    return ok;
}

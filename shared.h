// This is the code that is shared between nob.c and tests/*.c
#ifndef SHARED_H_
#define SHARED_H_

// Folder must end with forward slash /
#define BUILD_FOLDER "build/"
#define TESTS_FOLDER "tests/"
#define TOOLS_FOLDER "tools/"

bool build_exec(Cmd *cmd, const char *bin_path, const char *src_path)
{
#if defined(_MSC_VER)
    cmd_append(cmd, "cl", "-I.", "-o", bin_path, src_path);
#elif defined(__APPLE__) || defined(__MACH__)
    // TODO: "-std=c99", "-D_POSIX_SOURCE" didn't work for MacOS, don't know why, don't really care that much at the moment.
    //   Anybody who does feel free to investigate.
    cmd_append(cmd, "cc", "-Wall", "-Wextra", "-Wswitch-enum", "-I.", "-o", bin_path, src_path);
#else
    cmd_append(cmd, "cc", "-Wall", "-Wextra", "-Wswitch-enum", "-std=c99", "-D_POSIX_SOURCE", "-ggdb", "-I.", "-o", bin_path, src_path);
#endif //  _MSC_VER
    return cmd_run_sync_and_reset(cmd);
}

// Tests are allowed to build the tools they may need for their testing
// The tools are single C files residing in TOOLS_FOLDER
bool build_tool(Cmd *cmd, const char *tool_name)
{
    const char *tool_src = temp_sprintf("%s%s.c", TOOLS_FOLDER, tool_name);
    const char *tool_bin = temp_sprintf("%s%s", BUILD_FOLDER TOOLS_FOLDER, tool_name);
    return build_exec(cmd, tool_bin, tool_src);
}

#endif // SHARED_H_

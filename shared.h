// This is the code that is shared between nob.c and tests/*.c
#ifndef SHARED_H_
#define SHARED_H_

// Folder must end with forward slash /
#define BUILD_FOLDER "build/"
#define TESTS_FOLDER "tests/"
#define TOOLS_FOLDER "tools/"

// Tests are allowed to build the tools they may need for their testing
// The tools are single C files residing in TOOLS_FOLDER
bool build_tool(Cmd *cmd, const char *tool_name)
{
    const char *tool_src = temp_sprintf("%s%s.c", TOOLS_FOLDER, tool_name);
    const char *tool_bin = temp_sprintf("%s%s", BUILD_FOLDER TOOLS_FOLDER, tool_name);
    cmd_append(cmd, "cc", "-Wall", "-Wextra", "-Wswitch-enum", "-I.", "-o", tool_bin, tool_src);
    if (!cmd_run_sync_and_reset(cmd)) return false;
    return true;
}

#endif // SHARED_H_

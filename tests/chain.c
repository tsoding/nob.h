#include "shared.h"

Cmd cmd = {0};
Chain chain = {0};
Procs procs = {0};

const char *hello_src =
    "#include <stdio.h>\n"
    "\n"
    "int main()\n"
    "{\n"
    "    fprintf(stdout, \"Hello to stdout\\n\");\n"
    "    fprintf(stderr, \"Hello to stderr\\n\");\n"
    "    return 0;\n"
    "}\n";

const char *rot13_src =
    "#include <stdio.h>\n"
    "\n"
    "char rot13(char x)\n"
    "{\n"
    "    if ('a' <= x && x <= 'z') return ((x - 'a') + 13) % 26 + 'a';\n"
    "    if ('A' <= x && x <= 'Z') return ((x - 'A') + 13) % 26 + 'A';\n"
    "    return x;\n"
    "}\n"
    "\n"
    "#define BUFFER_SIZE (1024)\n"
    "\n"
    "char buffer[BUFFER_SIZE];\n"
    "\n"
    "int main(void)\n"
    "{\n"
    "    while (!feof(stdin)) {\n"
    "        size_t n = fread(buffer, sizeof(buffer[0]), BUFFER_SIZE, stdin);\n"
    "        for (size_t i = 0; i < n; ++i) {\n"
    "            buffer[i] = rot13(buffer[i]);\n"
    "        }\n"
    "        fwrite(buffer, sizeof(buffer[0]), n, stdout);\n"
    "    }\n"
    "\n"
    "    return 0;\n"
    "}\n";

const char *hex_src =
    "#include <assert.h>\n"
    "#include <stdio.h>\n"
    "#include <stdlib.h>\n"
    "#include <stdint.h>\n"
    "#include <ctype.h>\n"
    "\n"
    "#define COLUMNS 16\n"
    "#define PADDING 6\n"
    "\n"
    "uint8_t buffer[COLUMNS];\n"
    "\n"
    "int main(void)\n"
    "{\n"
    "    while (!feof(stdin)) {\n"
    "        size_t n = fread(buffer, sizeof(buffer[0]), COLUMNS, stdin);\n"
    "\n"
    "        for (size_t i = 0; i < n; ++i) {\n"
    "            printf(\"%02X \", buffer[i]);\n"
    "            if (!isprint(buffer[i])) buffer[i] = '.';\n"
    "        }\n"
    "\n"
    "        assert(n <= COLUMNS);\n"
    "        printf(\"%*s\", (int) (PADDING + (COLUMNS - n) * 3), \"\");\n"
    "\n"
    "        fwrite(buffer, sizeof(buffer[0]), n, stdout);\n"
    "        printf(\"\\n\");\n"
    "    }\n"
    "\n"
    "    return 0;\n"
    "}\n";

int main(void)
{
    if (!build_tool(&cmd, &procs, "./hello", "hello.c", hello_src)) return 1;
    if (!build_tool(&cmd, &procs, "./rot13", "rot13.c", rot13_src)) return 1;
    if (!build_tool(&cmd, &procs, "./hex", "hex.c", hex_src)) return 1;
    if (!procs_flush(&procs)) return 1;

    if (!chain_begin(&chain)) return 1;
    {
        cmd_append(&cmd, "./hello");
        if (!chain_cmd(&chain, &cmd, .err2out = true)) return 1;

        cmd_append(&cmd, "./rot13");
        if (!chain_cmd(&chain, &cmd)) return 1;

        cmd_append(&cmd, "./hex");
        if (!chain_cmd(&chain, &cmd)) return 1;
    }
    if (!chain_end(&chain,
        .async = &procs,
        .stdout_path = "output.txt",
    )) return 1;

    if (!procs_flush(&procs)) return 1;

    String_Builder sb = {0};
    if (!read_entire_file("output.txt", &sb)) return 1;

    String_View sv = sb_to_sv(sb);
    printf(SV_Fmt, SV_Arg(sv));

    return 0;
}

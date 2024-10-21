# nob.h

This library is the next generation of the [NoBuild](https://github.com/tsoding/nobuild) idea.

# Quick Example

The only file you need from here is [nob.h](https://raw.githubusercontent.com/tsoding/nob.h/refs/heads/main/nob.h). Just copy-paste it to your project and start using it.

```c
// nob.c
#define NOB_IMPLEMENTATION
#include "nob.h"

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);
    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-o", "main", "main.c");
    if (!nob_cmd_run_sync(cmd)) return 1;
    return 0;
}
```

```console
$ cc -o nob nob.c
$ ./nob
```

The `nob` automatically rebuilds itself if `nob.c` is modified thanks to
the `NOB_GO_REBUILD_URSELF` macro (don't forget to check out how it works in [nob.h](./nob.h))

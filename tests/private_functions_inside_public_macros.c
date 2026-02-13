// Some public functions may use private functions internally.
// Examples are NOB_GO_REBUILD_URSELF* and nob_cmd_append().
// Which means such private functions must be declared in the header part
// publically, even though their usage by the end users is discouraged.

#include "nob.h"

int main(void)
{
    Cmd cmd = {0};
    cmd_append(&cmd, "foo", "bar", "baz");
    da_foreach(const char *, arg, &cmd) {
        printf("%s\n", *arg);
    }
    return 0;
}

#define NOB_IMPLEMENTATION
#include "nob.h"

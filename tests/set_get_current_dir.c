#define NOB_IMPLEMENTATION
#include "nob.h"

int main(void)
{
    const char *test_current_dir = "/";

    nob_log(INFO, "Setting Current Dir to %s", test_current_dir);
    if (!set_current_dir(test_current_dir)) return 1;

    const char *current_dir = get_current_dir_temp();
    if (current_dir == NULL) return 1;
    nob_log(INFO, "Current Dir: %s", current_dir);
    return 0;
}

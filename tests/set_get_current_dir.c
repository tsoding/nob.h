#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

int main(void)
{
    const char *test_current_dir = "/";

    const char *current_dir = get_current_dir_temp();
    if (current_dir == NULL) return 1;
    nob_log(INFO, "Current Dir: %s", current_dir);

    nob_log(INFO, "Setting Current Dir to %s", test_current_dir);
    if (!set_current_dir(test_current_dir)) return 1;
    
    current_dir = get_current_dir_temp();
    if (current_dir == NULL) return 1;
    nob_log(INFO, "Current Dir: %s", get_current_dir_temp());
    return 0;
}

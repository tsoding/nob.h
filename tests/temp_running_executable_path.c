#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

int main(void)
{
    nob_log(INFO, "%s", temp_running_executable_path());
    return 0;
}

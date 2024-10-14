#define NOB_IMPLEMENTATION
#include "nob.h"

void log_some_messages(void)
{
    nob_log(NOB_INFO, "Info Message");
    nob_log(NOB_WARNING, "Warning Message");
    nob_log(NOB_ERROR, "Error Message");
}

int main(void)
{
    log_some_messages();
    nob_minimal_log_level = NOB_WARNING;
    log_some_messages();
    nob_minimal_log_level = NOB_ERROR;
    log_some_messages();
    nob_minimal_log_level = NOB_NO_LOGS;
    log_some_messages();
    return 0;
}

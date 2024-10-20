#define NOB_IMPLEMENTATION
#include "nob.h"

void log_test_messages(void)
{
    nob_log(NOB_INFO, "Info Test Message");
    nob_log(NOB_WARNING, "Warning Test Message");
    nob_log(NOB_ERROR, "Error Test Message");
    nob_log(NOB_NO_LOGS, "YOU SHOULD NEVER SEE THIS");
}

int main(void)
{
    log_test_messages();
    nob_minimal_log_level = NOB_WARNING;
    log_test_messages();
    nob_minimal_log_level = NOB_ERROR;
    log_test_messages();
    nob_minimal_log_level = NOB_NO_LOGS;
    log_test_messages();
    return 0;
}

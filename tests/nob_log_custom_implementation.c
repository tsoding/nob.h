#define NOB_IMPLEMENTATION
#define NOB_LOG_CUSTOM_IMPLEMENTATION
#include "nob.h"

const char * log_level_to_string_literal(Nob_Log_Level level)
{
    switch (level) {
    case NOB_INFO:      return "INFO";
    case NOB_WARNING:   return "WARNING";
    case NOB_ERROR:     return "ERROR";
    case NOB_NO_LOGS:   return "NO_LOGS";
    default:            return "unknown";
    }
}

// Custom implementation of nob_log
void nob_log(Nob_Log_Level level, const char *fmt, ...)
{
    fprintf(stderr, "[CUSTOM] Persistent Custom Log Test Message (Level: %s)\n",
        log_level_to_string_literal(level)
    );

    // You can still use the default implementation of nob_log
    va_list args;
    va_start(args, fmt);
    nob_log_default(level, fmt, args);
    va_end(args);
}

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
    return 0;
}

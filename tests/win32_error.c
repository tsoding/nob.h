#define NOB_IMPLEMENTATION
#include "nob.h"

int main(void) {
    nob_log(NOB_ERROR, "First 100 Win32 API Errors:");
    for (DWORD i = 0; i < 100; i++)
        nob_log(NOB_ERROR, "%u (0x%X): \"%s\"", (unsigned int)i, (unsigned int)i, nob_win32_error_message(i));
    return 0;
}

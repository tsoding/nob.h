#define NOB_IMPLEMENTATION
#include "nob.h"

int main(void) {
    for (DWORD i = 0; i < 16000; i++)
        nob_log(NOB_ERROR, "%lu (0x%X): \"%s\"", i, i, nob_log_win32_error(i));
    return 0;
}

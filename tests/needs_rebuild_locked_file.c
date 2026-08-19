#include "shared.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <time.h>
#endif

int main(void)
{
    // The harness runs tests in a fresh temp cwd.
    const char *output_path = "output.txt";
    const char *input_path  = "input.txt";

    // Write the output first, then the input, so the input is strictly
    // fresher than the output and needs_rebuild() must return 1.
    if (!write_entire_file(output_path, "output", 6)) return 1;

    // Make sure the input file gets a strictly fresher timestamp than the
    // output file regardless of the filesystem timestamp resolution.
#ifdef _WIN32
    Sleep(100);
#else
    struct timespec ts = {0, 100000000};
    nanosleep(&ts, NULL);
#endif

    if (!write_entire_file(input_path, "input", 5)) return 1;

#ifdef _WIN32
    // Hold the input file the way an editor or an AV scanner would. With
    // dwShareMode = 0 nobody else may open it, not even for reading, so
    // needs_rebuild() must not rely on opening the file.
    HANDLE lock = CreateFileA(input_path, GENERIC_READ, 0, NULL,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (lock == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "could not lock %s (err=%lu)\n", input_path, GetLastError());
        return 1;
    }
#endif // _WIN32

    // Must NOT fail just because the input file is locked.
    int result = needs_rebuild(output_path, &input_path, 1);
    printf("needs_rebuild = %d\n", result);

#ifdef _WIN32
    CloseHandle(lock);
#endif // _WIN32

    return 0;
}
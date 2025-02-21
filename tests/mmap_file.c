#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"
#include "shared.h"

int main(void) {
    int result = 0;

    const char *file_path = TESTS_FOLDER "/mmap_file.txt";

    String_Builder sb = {0};
    if (!read_entire_file(file_path, &sb)) return_defer(1);
    String_View content_via_read = sb_to_sv(sb);

    Mapped_File mf = mmap_file(file_path);
    if (mf.data == NULL) return_defer(1);
    String_View content_via_mmap = mf_to_sv(mf);

    if (!sv_eq(content_via_mmap, content_via_read)) {
        nob_log(ERROR, "Discrepancy between nob_read_entire_file() and nob_mmap_file()");
        return_defer(1);
    }

defer:
    free(sb.items);
    if (mf.data) munmap_file(mf);
    return result;
}

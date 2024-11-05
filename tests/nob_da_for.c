#define NOB_STRIP_PREFIX
#define NOB_IMPLEMENTATION
#include "nob.h"

typedef struct {
    float x, y, z;
} Vector3;

typedef struct {
    Vector3 *items;
    size_t count;
    size_t capacity;
} Various_Vector3s;


int main(void)
{
    const char *msg = tprintf("Short and Sweet `%s`", "tprintf");
    nob_log(INFO, "%s", msg);


    Nob_Cmd cmd = {0};
    cmd_append(&cmd, "jai.exe", "--virtual-by-default", "--allow-inheritance", "-Dnocheckin", "--activate-gc", "--enable-template", "first.jai");

#ifndef CC_HAS_TYPEOF_SUPPORT

    nob_log(INFO, "Compiler used has no typeof support. We can't use `it`, only can use `it_index`");
    da_for(&cmd) {
        const char* it = cmd.items[it_index];
        nob_log(INFO, "%d: `%s`", it_index, it);
    }
    return 0;

#else

    nob_log(INFO, "Iterating cmd parts");
    da_for(&cmd) {
        nob_log(INFO, "%d: `%s`", it_index, it);
    }


    Nob_File_Paths fps = {0};
    da_append(&fps, "D:\\sokoban");
    da_append(&fps, "/mnt/d/pfolder");

    nob_log(INFO, "Iterating %d file paths", fps.count);
    da_for(&fps) {
        nob_log(INFO, "%d: %s", it_index, it);
    }


    Various_Vector3s vecs = {0};
    da_append(&vecs, ((Vector3){1.0, 1.1, 1.2}));
    da_append(&vecs, ((Vector3){2.0, 2.1, 2.2}));
    da_append(&vecs, ((Vector3){3.0, 3.1, 3.2}));

    nob_log(INFO, "Iterating various %d Vector3s", vecs.count);
    da_for(&vecs) {
        nob_log(INFO, "%d: vec3{%.1f, %.1f, %.1f}", it_index, it.x, it.y, it.z, it_index);
    }

    return 0;
#endif
}

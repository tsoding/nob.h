#include "shared.h"
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

bool create_dirs()
{
    if (!nob_mkdir_if_not_exists("delete_dir")) return false;
    if (!nob_mkdir_if_not_exists("delete_dir/nested")) return false;
    if (!write_entire_file("delete_dir/bar.txt", NULL, 0)) return false;
    if (!write_entire_file("delete_dir/baz.txt", NULL, 0)) return false;
    if (!write_entire_file("delete_dir/nested/foo.txt", NULL, 0)) return false;
    if (!write_entire_file("delete_dir/nested/bar.txt", NULL, 0)) return false;
    if (!write_entire_file("delete_dir/nested/baz.txt", NULL, 0)) return false;
    return true;
}

bool delete_dir_without_symlinks()
{
    if (!create_dirs()) return false;
    if (!nob_delete_dir("delete_dir")) return false;
    if (nob_file_exists("delete_dir")) return false;
    return true;
}

bool delete_dir_with_symlinks()
{
    if (!create_dirs()) return false;
    if (symlink("delete_dir/nested", "delete_dir/nested/link") != 0) return false;
    if (!nob_delete_dir("delete_dir")) return false;
    if (nob_file_exists("delete_dir")) return false;
    return true;
}

int main(void)
{
    if (!delete_dir_without_symlinks()) return 1;
    if (!delete_dir_with_symlinks()) return 1;

    return 0;
}

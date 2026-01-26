#include "shared.h"

int main(void)
{
    printf("%s\n", temp_file_name(temp_running_executable_path()));
    return 0;
}

#include "raylib.h"
#define NOB_IMPLEMENTATION
#include "nob.h"

#define _RAY_PATH "../../raylib/raylib-5.0_linux_amd64"

int main(int argc, char** argv) 
{
    Nob_Cmd options = {0};
    nob_cmd_append(&options, "-Wall", "-Wextra", "-I../", "-I_RAYPATH/include");
    nob_cmd_append(&options, "-Wl,-rpath=_RAYPATH/lib/");
    nob_cmd_append(&options, "-L_RAY_PATH/Lib", "-l:libraylib.a");
    nob_cmd_append(&options, "-ldl", "-lm", "-lpthread"); 
    NOB_GO_REBUILD_URSELF_WITH_OPTIONS(argc, argv, &options);
    nob_cmd_free(options);
    
    const int width  = 800;
    const int height = 600;
    const char* program_name = nob_shift_args(&argc, &argv);
    InitWindow(width, height, program_name); 
    while (!WindowShouldClose()) {
        BeginDrawing();
         DrawCircle(width/2, height/2, 100, BLUE);
        EndDrawing();
    }
    CloseWindow();
    
    return 0;
}

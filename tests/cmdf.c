#define NOB_IMPLEMENTATION
#include "../nob.h"

int main(int argc, char** argv) {
	NOB_GO_REBUILD_URSELF(argc, argv);

	Nob_Cmd cmd = nob_cmdf(NULL, "cc -o cmdf cmdf.c");
	if(!nob_cmd_run_async_and_reset(&cmd)) return 1;
}

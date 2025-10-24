#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_DA_INIT_CAP 4
#include "nob.h"

int main(void){
	da_declare(int) xs;
	da_append(&xs , 10);

	nob_log(INFO , "Hello World %d " , xs.items[0]);
	return 0;
}

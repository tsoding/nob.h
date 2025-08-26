#define NOB_STRIP_PREFIX
#define NOB_IMPLEMENTATION
#include "nob.h"

int main(int argc, char **argv){
	NOB_GO_REBUILD_URSELF(argc, argv);

	#define build_dir "./debuild"

	mkdir_if_not_exists(build_dir);
	mkdir_if_not_exists(build_dir "/DEBIAN");
	mkdir_if_not_exists(build_dir "/usr");
	mkdir_if_not_exists(build_dir "/usr/lib");
	mkdir_if_not_exists(build_dir "/usr/include");
	copy_file("debian.control", build_dir "/DEBIAN/control");
	copy_file("nob.h", build_dir "/usr/include/nob.h");


	Cmd c = {0};
	nob_cc(&c);
	nob_cc_flags(&c);
	cmd_append(&c, "-fpic", "-shared", "-DNOB_IMPLEMENTATION", "-DNOB_LINK_URSELF");
	nob_cc_output(&c, build_dir "/usr/lib/libnob.so");
	cmd_append(&c, "-x", "c", "nob.h");
	if(!cmd_run(&c)) return 1;

	cmd_append(&c, "dpkg-deb", "-b", build_dir, "nob.h.deb");
	if(!cmd_run(&c)) return 1;

	#undef build_dir
}

#define NOB_STRIP_PREFIX

#include "nob.h"

void nob(print) (char* mes) {
    printf("> %s\n", mes);
}

void main() {
    nob(print)("This always compiles");

    nob_print("This compiles only with NOB_STRIP_PREFIX defined");
}

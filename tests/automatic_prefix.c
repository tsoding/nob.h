#define NOB_STRIP_PREFIX

#include "nob.h"

void nob(print) (char* mes) {
    printf("> %s\n", mes);
}

int main() {
    nob(print)("This always compiles");

    nob_print("This compiles only without NOB_STRIP_PREFIX defined");

    return 0;
}

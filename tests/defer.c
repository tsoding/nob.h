#define NOB_IMPLEMENTATION
#include "nob.h"

static void cleanup1(void* arg) {
    (void)arg;  // Unused
    nob_log(NOB_INFO, "Cleanup 1 executed");
}

static void cleanup2(void* arg) {
    (void)arg;  // Unused
    nob_log(NOB_INFO, "Cleanup 2 executed");
}

void test_basic_defer(void) {
    
    int dummy = 0;
    nob_defer(cleanup1, &dummy);
    nob_log(NOB_INFO, "First defer registered");
    
    {  // Testing nested scope
        int dummy2 = 0;
        nob_defer(cleanup2, &dummy2);
        nob_log(NOB_INFO, "Second defer registered in nested scope");
    }  // cleanup2 should execute here
}  // cleanup1 should execute here

void test_basic_memory(void) {
    void* ptr = malloc(100);
    if (!ptr) {
        nob_log(NOB_ERROR, "Memory allocation failed");
        return;
    }
    nob_defer_free(ptr);

    memset(ptr, 0, 100);
    nob_log(NOB_INFO, "Basic memory test completed");
}

int main(void) {
    test_basic_defer();
    test_basic_memory();
    return 0;
}

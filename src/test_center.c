#include <stdio.h>
#include "lar_unit.h"

void temp_set_up() {
    printf("Set Up activated!\n");
}

void temp_tear_down() {
    printf("Tear Down called!\n");
}

void some_test_func() {
    printf("This is a test being run!\n");
    LAR_EQUAL(5, 5);
}

int main() {
    printf("%s\n", __func__);
    printf("%s\n", __FILE__);
    printf("%i\n", __LINE__);
    LAR_INIT();
    MAKE_SET_UP(temp_set_up);
    MAKE_TEAR_DOWN(temp_tear_down);

    TEST(some_test_func);

    LAR_END();
    return 0;
}

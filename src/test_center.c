#include <stdio.h>
#include <stdint.h>

#include "lar_unit.h"
#include "test_two.h"

void temp_set_up() {
    printf("Set Up activated!\n");
}

void temp_tear_down() {
    printf("Tear Down called!\n");
}

void empty_test() {
    // To see if we get warned for no assertions.
}

void failed_test() {
    LAR_BOOL_EQUAL(true, false);
    int8_t five = 5;
    LAR_INT_EQUAL(five, 5);
    LAR_INT_EQUAL(5, 6);
}

void successful_test() {
    LAR_BOOL_EQUAL(true, true);
    LAR_INT_EQUAL(5, 5);
    LAR_INT_EQUAL(2, 2);
}

void bytes_array_test() {
    uint8_t bytesArray1[13];
    uint8_t bytesArray2[13];
    for (int i = 0; i * 20 < 255; i++) {
        bytesArray1[i] = i * 20;
    }
    for (int i = 0; i * 20 < 255; i++) {
        bytesArray2[i] = i * 20;
    }
    bytesArray2[8] = 9;
    LAR_BYTES_EQUAL(bytesArray1, bytesArray2, 12);
}

void string_array_test() {
    char str1[10] = "string!9", str2[10] = "string!";
    LAR_STRING_EQUAL(str1, str2);
}

int main() {
    LAR_INIT();
    MAKE_SET_UP(setUp2);
    MAKE_TEAR_DOWN(tearDown2);

    TEST(string_array_test2);
    TEST(failed_test);
    TEST(successful_test);
    TEST(empty_test);
    TEST(bytes_array_test);
    TEST(string_array_test);
    TEST(string_array_test2);

    LAR_END();
    return 0;
}

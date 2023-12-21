#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "lukip.h"
#include "included_tests.h"

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
    LUKIP_BOOL_EQUAL(true, false);
    int8_t five = 5;
    LUKIP_INT_EQUAL(five, 7);

    uint8_t bin1 = 112;
    unsigned long bin2 = 1546744073709551615;
    LUKIP_BINARY_EQUAL(bin1, bin2);

    LUKIP_HEX_EQUAL(0xA0, 0x9);
    LUKIP_INT_EQUAL(5, 6);
}

void successful_test() {
    LUKIP_BOOL_EQUAL(true, true);
    int8_t five = 5;
    LUKIP_INT_EQUAL(five, 5);
    LUKIP_HEX_EQUAL(0x9, 0x009);
    LUKIP_INT_EQUAL(2, 2);
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
    LUKIP_BYTES_EQUAL(bytesArray1, bytesArray2, 12);
}

void string_test() {
    char str1[10] = "string!9", str2[10] = "string!";
    LUKIP_STRING_EQUAL(str1, str2);
}

int main() {
    LUKIP_INIT();
    MAKE_SET_UP(set_up2);
    MAKE_TEAR_DOWN(tear_down2);

    TEST(string_test2);
    TEST(failed_test);
    TEST(successful_test);
    TEST(successful_test);

    // waste time
    for (int i = 0; i < 500000000; i++) {
        i++;
        i--;
    }
    
    TEST(empty_test);
    TEST(bytes_array_test);
    TEST(string_test);
    TEST(empty_test);
    TEST(string_test2);

    LUKIP_END();
    return 0;
}

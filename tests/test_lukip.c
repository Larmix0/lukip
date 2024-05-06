#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "lukip.h"
#include "included_tests.h"

TEST_FIXTURE(temp_set_up) {
    printf("Set Up activated!\n");
}

TEST_FIXTURE(temp_tear_down) {
    printf("Tear Down called!\n");
}

TEST_CASE(empty_test) {
    // To see if we get warned for no assertions.
}

TEST_CASE(failed_test) {
    ASSERT_BOOL_EQUAL(true, false);
    int8_t five = 5;
    ASSERT_INT_EQUAL(five, 7);

    uint8_t bin1 = 112;
    unsigned long long bin2 = 1546744073709551615;
    ASSERT_UBINARY_EQUAL(bin1, bin2);

    ASSERT_WITHIN_EQUAL(2.352, 2.356, 3);
    ASSERT_WITHIN_NOT_EQUAL(2.352, 2.356, 3);
    ASSERT_HEX_EQUAL(0xA0, 0x9);
    ASSERT_INT16_EQUAL(5, 6);
    ASSERT_CUSTOM(1 == 2, "Failed custom: %d == %d.", 1, 2);
}

TEST_CASE(successful_test) {
    ASSERT_BOOL_EQUAL(true, true);
    int8_t five = 5;
    ASSERT_INT_EQUAL(five, 5);
    ASSERT_HEX_EQUAL(0x9, 0x009);
    ASSERT_INT_EQUAL(2, 2);
}

TEST_CASE(bytes_array_test) {
    uint8_t bytesArray1[13];
    uint8_t bytesArray2[13];
    for (int i = 0; i * 20 < 255; i++) {
        bytesArray1[i] = i * 20;
    }
    for (int i = 0; i * 20 < 255; i++) {
        bytesArray2[i] = i * 20;
    }
    bytesArray2[8] = 9;
    ASSERT_BYTES_EQUAL(bytesArray1, bytesArray2, 12);
}

TEST_CASE(string_test) {
    char str1[10] = "string1", str2[10] = "string!";
    ASSERT_STRING_EQUAL(str1, str2);
}

int main() {
    LUKIP_INIT();
    MAKE_SETUP(set_up2);
    MAKE_TEARDOWN(tear_down2);

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

    return 0;
}

/**
 * @file test_main.c
 * @brief Main file for testing Lukip's functionality.
 * 
 * @author Larmix
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "lukip.h"
#include "included_tests.h"

/** General setup for the tests. */
DECLARE_SETUP(test_setup) {
    printf("Setup activated.\n");
}

/** General teardown for the tests. */
DECLARE_TEARDOWN(test_teardown) {
    printf("Teardown called.\n");
}

/** Just an empty tests to ensure we get warned for having no assertions. */
TEST_CASE(empty_test) {
}

/** A test which ends up with some different kinds of failures. */
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

/** A test with a variety of successful assertions. */
TEST_CASE(successful_test) {
    ASSERT_BOOL_EQUAL(true, true);
    int8_t five = 5;
    ASSERT_INT_EQUAL(five, 5);
    ASSERT_HEX_EQUAL(0x9, 0x009);
    ASSERT_INT_EQUAL(2, 2);
}

/** A test for bytecode array assertions. */
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

/** Tests that raising failures or warnings works properly (with custom and default messages). */
TEST_CASE(raise_test) {
    ASSERT_RAISE_FAIL_MESSAGE("Fail raise test.");
    ASSERT_RAISE_FAIL();
    ASSERT_RAISE_WARN_MESSAGE("Raise warn test.");
    ASSERT_RAISE_WARN();
}

/** Tests string assertions works properly. */
TEST_CASE(string_test) {
    char str1[10] = "string1", str2[10] = "string!";
    ASSERT_STRING_EQUAL(str1, str2);
}

/** Main entrance point of Lukip unit testing. */
int main() {
    LUKIP_INIT();
    MAKE_SETUP(set_up2);
    MAKE_TEARDOWN(tear_down2);

    printf("Status code: %d (expecting non-failure).\n", LUKIP_STATUS());

    TEST(string_test2);
    TEST(failed_test);
    TEST(raise_test);
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

    printf("Status code: %d (expecting failure).\n", LUKIP_STATUS());
    return 0;
}

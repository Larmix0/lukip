/**
 * @file included_tests.c
 * @brief Source file of tests that are included from a different file.
 * 
 * @author Larmix
 */

#include <stdio.h>
#include <stdint.h>

#include "lukip.h"

int globalNum = 0; /** Global variable to modify during testing. */

/** Setup from a different file. */
DECLARE_SETUP(set_up2) {
    globalNum += 2;
}

/** Teardown from a different file. */
DECLARE_TEARDOWN(tear_down2) {
    globalNum -= 1;
}

/** A random test from a different file. A string comparison test in this case. */
TEST_CASE(string_test2) {
    printf("Global num is: %d.\n", globalNum);
    char str1[10] = "string!9", str2[10] = "string!";
    ASSERT_STRING_EQUAL(str1, str2);
}

#include <stdio.h>
#include <stdint.h>

#include "lukip.h"

int globalNum = 0;

DECLARE_SETUP(set_up2) {
    globalNum += 2;
}

DECLARE_TEARDOWN(tear_down2) {
    globalNum -= 1;
}

TEST_CASE(string_test2) {
    printf("Global num is: %d.\n", globalNum);
    char str1[10] = "string!9", str2[10] = "string!";
    ASSERT_STRING_EQUAL(str1, str2);
}

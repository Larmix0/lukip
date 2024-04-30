#include <stdio.h>
#include <stdint.h>

#include "lukip.h"

int globalNum = 0;

TEST_FIXTURE(set_up2) {
    globalNum += 2;
}

TEST_FIXTURE(tear_down2) {
    globalNum -= 1;
}

TEST_CASE(string_test2) {
    printf("Global num is: %d\n", globalNum);
    char str1[10] = "string!9", str2[10] = "string!";
    ASSERT_STRING_EQUAL(str1, str2);
}

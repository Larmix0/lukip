#include <stdio.h>
#include <stdint.h>

#include "lar_unit.h"

int globalNum = 0;

void setUp2() {
    globalNum += 2;
}

void tearDown2() {
    globalNum -= 1;
}

void string_array_test2() {
    printf("Global num is: %d\n", globalNum);
    char str1[10] = "string!9", str2[10] = "string!";
    LAR_STRING_EQUAL(str1, str2);
}

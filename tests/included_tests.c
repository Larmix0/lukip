#include <stdio.h>
#include <stdint.h>

#include "../src/lukip.h"

int globalNum = 0;

void set_up2() {
    globalNum += 2;
}

void tear_down2() {
    globalNum -= 1;
}

void string_test2() {
    printf("Global num is: %d\n", globalNum);
    char str1[10] = "string!9", str2[10] = "string!";
    LUKIP_STRING_EQUAL(str1, str2);
}

#include <stdint.h>
#include <string.h>

#include "lar_unit_impl.h"
#include "lar_asserts.h"

void assert_bytes_equal(
    void *array1, void *array2, const int length,
    char *fileName, char *funcName, const int line
) {
    TestInfo info = {.fileName=fileName, .funcName=funcName, .status=UNKNOWN};
    uint8_t arr1Byte, arr2Byte;

    for (int i = 0; i < length; i++) {
        arr1Byte = ((uint8_t *)array1)[i]; 
        arr2Byte = ((uint8_t *)array2)[i];
        if (arr1Byte == arr2Byte) {
            continue;
        }
        char *message;
        FORMAT_ALLOC(
            message,
            "Failed at index %i: %u != %u. (Expected equal).", i, arr1Byte, arr2Byte
        );
        assert_failure(info, line, message);
        return;
    }
    assert_success(info);
}

void assert_strings_equal(
    char *string1, char *string2,
    char *fileName, char *funcName, const int line
) {
    size_t length1 = strlen(string1);
    size_t length2 = strlen(string2);
    TestInfo info = {.fileName=fileName, .funcName=funcName, .status=UNKNOWN};

    char *message;
    if (length1 != length2) {
        FORMAT_ALLOC(
            message,
            "Different lengths: %zu != %zu. (Expected same strings)", length1, length2
        );
        assert_failure(info, line, message);
        return;
    }
    if (strncmp(string1, string2, strlen(string1)) != 0) {
        FORMAT_ALLOC(message, "%s != %s. (Expected same strings)", string1, string2);
        assert_failure(info, line, message);
        return;
    }
    assert_success(info);
}

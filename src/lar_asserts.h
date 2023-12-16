#ifndef LAR_ASSERTS_H
#define LAR_ASSERTS_H

// use first instead of allocating 256,
// then strncpy into heap allocated message
// after allocating with strlen(buf); 
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LENGTH 256

#define FORMAT_ALLOC(message, format, ...) \
    do { \
        char buffer[256]; \
        snprintf(buffer, BUFFER_LENGTH, format, __VA_ARGS__); \
        const int len = strlen(buffer); \
        message = malloc(len * sizeof(char)); \
        strncpy(message, buffer, len); \
    } while (0)

void assert_bytes_equal(
    void *array1, void *array2, const int length,
    char *fileName, char *funcName, const int line
);
void assert_strings_equal(
    char *string1, char *string2,
    char *fileName, char *funcName, const int line
);

#endif

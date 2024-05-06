#include <stdio.h>

#include "allocator.h"

/**
 * @brief Allocates from the heap and checks for NULL itself. 
 * 
 * @param size The amount of elements to be allocated.
 * @param elementSize The size of each element.
 * 
 * @return Allocated pointer.
 */
void *allocate(const int size, const size_t elementSize) {
    void *result = malloc(size * elementSize);
    if (result == NULL) {
        printf("Lukip failed to allocate memory.");
        exit(1);
    }
    return result;
}

/**
 * @brief Reallocates a certain heap pointer and checks for NULL itself.
 * 
 * @param pointer The pointer to be reallocated.
 * @param newSize The new amount of elements it should be able to hold.
 * @param elementSize The size of each element it holds.
 * 
 * @return The new reallocated pointer.
 */
void *reallocate(void *pointer, const int newSize, const size_t elementSize) {
    void *result = realloc(pointer, newSize * elementSize);
    if (result == NULL) {
        printf("Lukip failed to reallocate a block of memory.");
        exit(1);
    }
    return result;
}

#include "stdlib.h"

/**
 * @brief Allocates from the heap and checks for NULL itself. 
 * 
 * @param size The amount of elements to be allocated.
 * @param elementSize The size of each element.
 * 
 * @return Allocated pointer.
 */
void *allocate(const int size, const size_t elementSize);

/**
 * @brief Reallocates a certain heap pointer and checks for NULL itself.
 * 
 * @param pointer The pointer to be reallocated.
 * @param newSize The new amount of elements it should be able to hold.
 * @param elementSize The size of each element it holds.
 * 
 * @return The new reallocated pointer.
 */
void *reallocate(void *pointer, const int newSize, const size_t elementSize);

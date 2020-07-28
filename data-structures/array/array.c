// array.c
// A data structure that provides an array-like interface
// but supports transparent dynamic resizing.

#include "array.h"

array_t *array_new(
    size_t min_capacity,
    deleter_f deleter)
{
    return NULL;
}

void array_delete(array_t *array)
{
    return;
}

void* array_get(array_t *array, size_t index)
{
    return NULL;
}

void array_put(array_t *array, size_t index, void *value)
{
    return;
}

void array_erase(array_t *array, size_t index)
{
    return;
}

ssize_t array_capacity(array_t *array)
{
    return -1;
}

ssize_t array_count(array_t *array)
{
    return -1;
}
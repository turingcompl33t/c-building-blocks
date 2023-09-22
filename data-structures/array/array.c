// array.c
// A data structure that provides an array-like interface
// but supports transparent dynamic resizing.

#define _GNU_SOURCE
#include "array.h"

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

// ----------------------------------------------------------------------------
// Internal Declarations

struct array
{
    // The internal dynamic buffer used to implement the array.
    void** buffer;

    // The current count of items in the array.
    size_t count;

    // The current total capacity of the array.
    size_t capacity;

    // The minimum capacity of the array.
    size_t min_capacity;    

    // The greatest index in the array currently in use.
    size_t max_index;

    // The user-provided delete function.
    deleter_f deleter;
};

static bool grow_array(array_t* array, size_t required_capacity);
static bool shrink_array_if_possible(array_t* array);

static bool out_of_range(array_t* array, size_t index);

static size_t max(size_t a, size_t b);

static size_t prev_in_use_index(array_t* array, size_t index);

static size_t next_power_of_two(size_t v);
static size_t next_power_of_two_32(size_t v);
static size_t next_power_of_two_64(size_t v);

// ----------------------------------------------------------------------------
// Exported

array_t* array_new(
    size_t    min_capacity, 
    deleter_f deleter)
{
    if (0 == min_capacity || NULL == deleter)
    {
        return NULL;
    }

    array_t* array = malloc(sizeof(array_t));
    if (NULL == array)
    {
        return NULL;
    }

    const size_t initial_capacity = next_power_of_two(min_capacity);

    void** buffer = calloc(initial_capacity, sizeof(void*));
    if (NULL == buffer)
    {
        free(array);
        return NULL;
    }

    array->buffer       = buffer;
    array->count        = 0;
    array->capacity     = initial_capacity;
    array->min_capacity = initial_capacity;
    array->max_index    = 0;
    array->deleter      = deleter;

    return array;
}

void array_delete(array_t* array)
{
    if (NULL == array)
    {
        return;
    }

    for (size_t i = 0; i < array->capacity; ++i)
    {
        if (array->buffer[i] != NULL)
        {
            array->deleter(array->buffer[i]);
        }
    }

    free(array);
}

void* array_get(array_t* array, size_t index)
{
    if (NULL == array)
    {
        return NULL;
    }

    return out_of_range(array, index)
        ? NULL 
        : array->buffer[index];
}

void array_put(array_t* array, size_t index, void* value)
{
    if (NULL == array)
    {
        return;
    }

    if (out_of_range(array, index))
    {
        // require a resize; expand the array minimum capacity
        // necessary to accomodate the requested index
        assert(grow_array(array, index + 1));
    }

    // destroy an existing item if present
    if (array->buffer[index] != NULL)
    {
        array->deleter(array->buffer[index]);
    }
    else
    {
        array->count++;
        array->max_index = max(index, array->max_index);
    }

    array->buffer[index] = value;
}

void array_erase(array_t* array, size_t index)
{
    if (NULL == array || out_of_range(array, index))
    {
        return;
    }

    // empty entry
    if (NULL == array->buffer[index])
    {
        return;
    }

    // clear the entry
    array->deleter(array->buffer[index]);
    array->buffer[index] = NULL;

    if (index == array->max_index)
    {
        // this is the maximum in-use index in the array;
        // determine if we can shrink the capacity of the array
        array->max_index = prev_in_use_index(array, index);
        assert(shrink_array_if_possible(array));
    }

    array->count--;
}

ssize_t array_capacity(array_t* array)
{
    return (NULL == array) ? -1 : array->capacity;
}

ssize_t array_count(array_t* array)
{
    return (NULL == array) ? -1 : array->count;
}

// ----------------------------------------------------------------------------
// Internal

// Expand the array to accomodate `required_capacity`.
static bool grow_array(array_t* array, size_t required_capacity)
{
    assert(required_capacity > array->capacity);

    const size_t new_capacity = next_power_of_two(required_capacity);

    void** new_buffer = reallocarray(array->buffer, new_capacity, sizeof(void*));
    if (NULL == new_buffer)
    {
        return false;
    }

    // initialize the new array
    for (size_t i = array->max_index + 1; i < new_capacity; ++i)
    {
        new_buffer[i] = NULL;
    }

    array->buffer   = new_buffer;
    array->capacity = new_capacity;

    return true;
}

// Attempt to shrink the array.
static bool shrink_array_if_possible(array_t* array)
{
    // compute the next lower possible capacity for the array
    const size_t next_lower_capacity 
        = max(array->min_capacity, (array->capacity >> 1));
    
    if (array->max_index < next_lower_capacity 
        && next_lower_capacity < array->capacity)
    {
        void** new_buffer = reallocarray(array->buffer, next_lower_capacity, sizeof(void*));
        if (NULL == new_buffer)
        {
            return false;
        }

        array->buffer   = new_buffer;
        array->capacity = next_lower_capacity;
    }

    return true;
}

// Compute maximum of `a` and `b`.
static size_t max(size_t a, size_t b)
{
    return (a > b) ? a : b;
}

// Determine the previous in-use index in the array.
static size_t prev_in_use_index(array_t* array, size_t index)
{
    for (size_t i = index; i > 0; i--)
    {
        if (array->buffer[i] != NULL)
        {
            return i;
        }
    }

    return 0;
}

// Determine if the provided index is out of range.
static bool out_of_range(array_t* array, size_t index)
{
    return (index >= array->capacity);
}

// Compute the next greater power of 2.
static size_t next_power_of_two(size_t v)
{
    return (sizeof(size_t) == 8) 
        ? next_power_of_two_64(v) 
        : next_power_of_two_32(v);
}

// https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
static size_t next_power_of_two_32(size_t v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    return ++v;
}

// https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
static size_t next_power_of_two_64(size_t v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v |= v >> 32;
    return ++v;
}
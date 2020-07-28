// array.h
// A data structure that provides an array-like interface
// but supports transparent dynamic resizing.

#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#include <sys/types.h>

typedef struct array array_t;

// The signature for the user-provided delete function.
typedef void (*deleter_f)(void *);

// array_new()
//
// Construct a new array.
//
// Arguments:
//  min_capacity - the minimum capacity of the array;
//                 the array guarantees that it will never
//                 resize such that it cannot accomodate
//                 `min_capacity` items without growing.
//  deleter      - user provided delete function
//
// Returns:
//  A pointer to a new array instance on success
//  NULL on failure
array_t* array_new(
    size_t min_capacity,
    deleter_f deleter);

// array_delete()
//
// Destroy an array instance.
//
// Arguments:
//  array - pointer to an existing array instance
void array_delete(array_t *array);

// array_get()
//
// Retrieve the item at index `index` in the array.
//
// If `index` is out of range, or no item was inserted
// at `index` via array_put(), this function returns NULL.
//
// Arguments:
//  array - pointer to an existing array instance
//  index - the index at which to query the item
//
// Returns:
//  The value stored at `index`
//  NULL on failure
void* array_get(array_t *array, size_t index);

// array_put()
//
// Insert `value` into the array at index `index`.
//
// If `index` already contains an item, the item 
// at `index` is destroyed using the user-provided
// delete function and the new `value` is inserted
// in its place.
//
// If `index` is currently out of range, the array 
// resizes such that it is able to accomodate inserting
// `value` at the specified index.
//
// Arguments:
//  array - pointer to an existing array instance
//  index - the index at which `value` should be inserted
//  value - the value to be inserted
void array_put(array_t *array, size_t index, void *value);

// array_erase()
//
// Erase the item at `index` in the array.
//
// If `index` currently does not contain an item
// inserted via array_put(), this function is a no-op.
//
// If after removing the item at the provided index
// the array determines that it can safely shrink and
// still accomodate the remainder of its contents, it
// performs a resize operation. However, the array never
// shrinks its capacity below that value specified in 
// its constructor.
//
// Arguments:
//  array - pointer to an existing array instance
void array_erase(array_t *array, size_t index);

// array_capacity()
//
// Query the current capacity of the array.
//
// Arguments:
//  array - pointer to an existing array instance
//
// Returns:
//  The current capacity of the array on success
//  -1 on failure
ssize_t array_capacity(array_t *array);

// array_count()
//
// Query the current count of items in the array.
//
// Arguments:
//  array - pointer to an existing array instance
//
// Returns:
//  The current count of items in the array on success
//  -1 on failure
ssize_t array_count(array_t *array);

#endif // ARRAY_H
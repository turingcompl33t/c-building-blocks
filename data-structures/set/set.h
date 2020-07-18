// set.h
// Generic set data structure.

#ifndef SET_H
#define SET_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// The hash type.
//
// In this context, a "hash" merely refers to a way
// to unique identify an item in the set.
typedef uint64_t hash_t;

// A user-provider delete function.
//
// The expected semantics for this function are:
//  given a pointer to an arbitrary data type that
//  the user intends to store in the set, invoking
//  this function deallocates all of the storage 
//  for the specified item.
typedef void (*deleter_f)(void*);

// A user-provider hash function.
//
// The expected semantics for this function are:
//  given a pointer to an arbitrary data type that
//  the user intends to store in the set, invoking 
//  this function returns a hash of the provided 
//  data that enables the set to internally identify
//  the item.
typedef hash_t (*hasher_f)(void*);

// A user-provider iterator function.
// 
// This function signature is the signature
// expected by set_for_each(). The function is
// invoked on the user data for each item in the set.
typedef void (*iterator_f)(void*);

struct set_item;

typedef struct set
{
    // Pointer to the head of the internal linked-list
    struct set_item* head;

    // The delete function.
    deleter_f deleter;

    // The hash function.
    hasher_f hasher;

    // Total count of items currently in the set
    size_t count;
} set_t;

// set_new()
//
// Constructs a new set data structure.
//
// Arguments:
//  deleter - user-provided delete function
//  hasher  - user-provided hash function
//
// Returns:
//  A pointer to a newly constructed set on success
//  NULL on failure
set_t* set_new(deleter_f deleter, hasher_f hasher);

// set_delete()
//
// Destroys an existing set data structure.
//
// Assuming it receives valid input, this function 
// ensures that all memory utilized by the set
// data structure is deallocated at the time it returns.
//
// Arguments:
//  set - pointer to existing set data structure 
void set_delete(set_t* set);

// set_add()
//
// Add a new item to the set.
//
// This function hashes the user-provided
// data and attempt to insert the data into the
// set. In the event that the item already exists
// in the set, this function DOES NOT insert the 
// new item and return `false`. 
//
// Arguments:
//  set  - pointer to existing set data structure
//  data - user provided data to store
bool set_add(set_t* set, void* data);

// set_remove()
//
// Remove an item from the set.
//
// This function hashes the user-provided
// data and attempts to remove the data 
// from the set. In the event that the item
// is not found, this function returns `false`.
//
// Note, however, that this function DOES NOT
// attempt to deallocate the user-provided data,
// even though it has the ability to because the
// set maintains an appropriate delete function.
// The user must pass a pointer to the removed
// data to this function in order to remove it,
// thus freeing the data out from under the user
// would present a poor API decision (in my view).
//
// Arguments:
//  set  - pointer to existing set data structure
//  data - user provided data to remove
//
// Returns:
//  `true` on successful removal
//  `false` if item not found
bool set_remove(set_t* set, void* data);

// set_contains()
//
// Determine if the set contains an item.
//
// Arguments:
//  set  - pointer to existing set data structure
//  data - user-provided data for which to search 
//
// Returns:
//  `true` if the item is present in the set
//  `false` otherwise
bool set_contains(set_t* set, void* data);

// set_count()
//
// Returns the number of items in the set.
//
// Arguments:
//  set - pointer to existing set data structure
//
// Returns:
//  The count of items in the set
//  0 on failure
size_t set_count(set_t* set);

// set_for_each()
//
// Invoke the specified iterator function
// on each item in the set.
//
// Arguments:
//  set      - pointer to existing set data structure
//  iterator - the function invoked on each set item
void set_for_each(set_t* set, iterator_f iterator);

#endif // SET_H
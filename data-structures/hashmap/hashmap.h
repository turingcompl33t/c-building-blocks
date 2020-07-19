// hashmap.h
// Generic hashmap data structure.

#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Background:
//
// TODO

// The type returned by the hash function utilized by this table.
typedef uint64_t hash_t;

// The signature for the user-provided hash function
// that accepts a key and produces a hash.
typedef hash_t (*hasher_f)(void*);

// The signature for the user-provided comparison
// function that accepts a pair of keys and 
// return `true` if the two are equal, `false` otherwise. 
typedef bool (*comparator_f)(void*, void*);

// The signature for the user-provided delete
// function that accepts a value and
// deallocates all of the memory for it.
typedef void (*deleter_f)(void*);

struct bucket;

// The hashmap data structure.
typedef struct hashmap
{
    // The user-provided operations.
    hasher_f     hasher;
    comparator_f comparator;
    deleter_f    deleter;

    // The array of buckets that composes the internal table.
    struct bucket* buckets;

    // The current length of the bucket array.
    size_t n_buckets;

    // The total number of items in the map.
    size_t count;
} hashmap_t;

// hashmap_new()
// 
// Construct a new hashmap structure.
//
// This function validates its arguments and does
// not return a valid table if any of the provided
// callback functions are invalid. 
//
// Arguments:
//  hasher     - user-provided hash function
//  comparator - user-provided comparison function
//  deleter    - user-provided delete function
//
// Returns:
//  A pointer to a newly constructed hashmap on success
//  NULL on failure (invalid arguments, allocation failure)
hashmap_t* hashmap_new(
    hasher_f hasher, 
    comparator_f comparator, 
    deleter_f deleter);

// hashmap_delete()
//
// Destroys an existing hashmap structure.
//
// When this function returns, all of the memory
// utilized by the hashmap is deallocated. This 
// implies that any outstanding pointers to
// values stored in the map are invalidated.
//
// Arguments:
//  map - pointer to existing hashmap strucure
void hashmap_delete(hashmap_t* map);

// hashmap_insert()
//
// Insert a new key, value pair into the hashmap.
//
// The semantics of the insertion operation are as follows:
// - if the given key DOES NOT already exist in the map, 
//   the new value is inserted under that key, and `true` is returned
// - otherwise, the given key ALREADY EXISTS in the map,
//   the new value is inserted under that key, and the old
//   value associated with that key is returned via the out
//   parameter `out` (if non-NULL), and `true` is returned 
//
// NOTE: The semantics of this map are such that it DOES NOT
// assume ownership of either the keys or values inserted
// into it. That is, it is entirely possible for users to 
// invalidate pointers that are stored in this map from
// outside the map itself. The map cannot protect against this
// because it accepts only generic arguments. 
//
// Arguments:
//  map   - pointer to existing hashmap strucure
//  key   - key under which to perform the insertion
//  value - value to insert under key
//  out   - set to old value if key collision occurs
//
// Returns:
//  `true` in the event that insertion of the value succeeded
//  `false` otherwise
bool hashmap_insert(
    hashmap_t* map, 
    void* key, 
    void* value, 
    void** out);

// map_remove()
//
// Remove the value associated with the given `key`.
//
// In the event that the given key is a valid key
// into the map, this function destroys the value
// associated with `key` using the user-provided 
// delete function and removes the key from the map.
//
// NOTE: The semantics of this map are such that it
// will never try to destroy a key type that it maintains.
// This is to account for the most common cases in which
// types such as integral types or strings (static) are
// used as keys into the map. 
//
// Arguments:
//  map - pointer to existing hashmap strucure
//  key - the key for the value to remove
//
// Returns:
//  `true` on successful removal of the value associated with `key`
//  `false` otherwise
bool hashmap_remove(hashmap_t* map, void* key);

// hashmap_find()
//
// Searches the map for the value associated with `key`.

// Arguments:
//  map - pointer to existing hashmap strucure
//  key - the key associated with the value for which to search
//
// Returns:
//  A pointer to the value associated with `key` if found
//  NULL otherwise
void* hashmap_find(hashmap_t* map, void* key);

// hashmap_count()
//
// Returns the total count of items in the map. 
//
// Arguments:
//  map - pointer to existing hashmap strucure
//
// Returns:
//  The total count of items in the map.
size_t hashmap_count(hashmap_t* map);

#endif // HASHMAP_H
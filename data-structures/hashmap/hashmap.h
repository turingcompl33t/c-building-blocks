// hashmap.h
// Generic hashmap data structure.

#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "intrusive_list.h"

// Background:
//
// A map is a general abstraction for a key / value store.
// With a map, we can insert a value keyed by the associated
// key, and subsequently retrieve, modify, or remove this
// value with knowledge of the key only. Maps are incredibly
// important data structures that occur in countless subfields
// of computer science.
//
// A hashmap is one possible implementation of the map API in
// which keys are passed through a hash function in order to 
// derive an index into an internal table managed by the map.
// Such a procedure is useful because it allows for efficient
// insert, remove, and search operations that operate in 
// constant time (O(1)) rather than variations that scale with
// the number of items in the map (O(n)). 
//
// The above description does have one complication, however:
// what happens in the event that two keys hash to the same
// value? In this case, both key / value pairs map to the same
// bucket in the table, and we experience what is known as a
// collision. There are many different hashmap implementations 
// that implement various methods of dealing with this situation.
// One common and relatively simple method for dealing with collisions
// is a technique known as separate chaining. In separate chaining,
// each bucket is not a single item in the hashtable, but rather
// is the head of a list of items that map to that particular bucket.
// When two or more keys map to the same bucket in the table,
// we simply extend the list for that bucket with the new item.
//
// Separate chaining is the collision management technique
// utilized in this particular hashmap implementation.
//
// One other complication that arises in hashmap implementation
// is the management of table resize operations. Above, I claimed
// that insert, remove, and find operations on a hashmap operate 
// in constant time. This is true so long as the number of items 
// that map to each particular bucket in the table is small. As 
// the number of items in a bucket grows large, we begin to require
// a greater number of linear searches through the bucket lists
// when performing any of the hashmap operations, degrading the
// expected performance of the hashmap to time linear in the number
// of items in the map - exactly what the hashmap was meant to avoid!
//
// The remedy for this issue is periodic resizing of the internal
// table that the hashmap uses to manage its items. Once the number
// of items in the table exceeds a certain threshold, the number of
// buckets in the table is increased and the table's items are 
// re-inserted into this new table. Such resize operations are relatively
// expensive, but heuristics are typically employed to ensure that they
// occur less and less frequently as the size of the hashmap continues
// to grow.
//
// For the purposes of this implementation, utilize the following
// parameters and heuristics to determine hashmap resize behavior:
//
// - The desired load factor of the table should be set at 0.75.
//   The load factor of a hashmap is the threshold percentage of 
//   the overal capacity of the table (number of buckets) above which 
//   a resize operation will take place. A load factor of 0.75 
//   is widely recognized as providing generally competetive performance.
// - In accordance with the above description of the desired load
//   factor, a resize of the hashmap should take place when:
//
//              N_ITEMS >= 0.75*CAPACITY
// 
// - On hashmap resize, the total capacity of the table (the 
//   number of buckets) should double. Again, this parameter is
//   widely recognized to provide solid performance as it amortizes
//   the cost of expensive resize operations.
//
// Finally, it was mentioned above that a list is typically used
// to handle hash collisions. A good choice for this list implementation
// is an "intrusive list" - see the accompanying intrusive_list.h
// header for more details.
//
// It is generally recommended that one complete the intrusive list
// implementation prior to beginning work on the hashmap itself.

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

// Buckets in the internal table are represented as 
// the heads of intrusive linked-list structures.
typedef list_entry_t bucket_t;

// The hashmap data structure.
typedef struct hashmap
{
    // The user-provided operations.
    hasher_f     hasher;
    comparator_f comparator;
    deleter_f    deleter;

    // The array of buckets that composes the internal table.
    bucket_t* buckets;

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
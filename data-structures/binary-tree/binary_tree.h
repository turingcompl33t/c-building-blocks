// binary_tree.h
// Generic binary search tree data structure.

#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stddef.h>
#include <stdbool.h>

typedef enum 
{
	LESS,
	EQUAL,
	GREATER
} ordering_t;

// The signature for the user-provided comparison function.
//
// This function accepts two keys of arbitrary type,
// key `a` (the former argument) and key `b` (the latter).
//
// The function must then returns an ordering in 
// accordance with the following rules:
//	
// - If `a` is less than `b`, LESS is returned
// - If `a` and `b` are equivalent, EQUAL is returned
// - If `a` is greater than `b`, GREATER is returned
typedef ordering_t (*comparator_f)(void*, void*);

// The signature for the user-provider delete function.
//
// Once this function completes, all storage for the
// user-provided data identified by the function's argument
// is deallocated. Outstanding pointers to this data
// are invalidated.
typedef void (*deleter_f)(void*);

// The signature for the user-provided iterator function.
//
// In tree_for_each(), this function is invoked on each
// key / pair stored in the tree in a pre-order traversal.
typedef void (iterator_f)(void*, void*);

struct treenode;

// The binary tree structure.
typedef struct binary_tree
{
	// The user-provider comparison function.
	comparator_f comparator;

	// The user-provided delete function.
	deleter_f deleter;

	// The current total count of items in the tree.
	size_t count;

	// The root of the binary tree.
	struct treenode* root;
} binary_tree_t; 

// tree_create()
//
// Construct a new binary tree structure.
//
// Arguments:
//	comparator - user-provided comparison function
//	deleter    - user-provided deleter function
//
// Returns:
//	A pointer to newly constructed binary tree on success
//	NULL on failure (invalid arguments, allocation failure)
binary_tree_t* tree_new(comparator_f comparator, deleter_f deleter); 

// tree_delete()
//
// Destroy an existing binary tree structure.
//
// This function destroys all of the user-provided
// data currently stored in the tree. Therefore,
// once this function completes, any outstanding
// pointers to data stored in the tree are invalidated.
//
// Arguments:
//	tree - pointer to existing tree data structure
void tree_delete(binary_tree_t *tree);

// tree_insert()
//
// Insert a new key / value pair into the tree.
//
// The semantics of the insertion operation are as follows:
// - if the given key DOES NOT already exist in the tree, 
//   the new value is inserted under that key, and `true` is returned
// - otherwise, the given key ALREADY EXISTS in the tree,
//   the new value is inserted under that key, and the old
//   value associated with that key is returned via the out
//   parameter `out` (if non-NULL), and `true` is returned 
//
// NOTE: The semantics of this tree are such that it DOES NOT
// assume ownership of either the keys or values inserted
// into it. That is, it is entirely possible for users to 
// invalidate pointers that are stored in this map from
// outside the map itself. The map cannot protect against this
// because it accepts only generic arguments. 
// 
// Arguments:
//	tree  - pointer to existing tree data structure
//	key   - the key under which to insert
//  value - the value to associate with the given key
//  out   - set to previous value if key collision occurs
//
// Returns:
//	`true` if the given value is inserted into the tree
//  `false` otherwise
bool tree_insert(
	binary_tree_t* tree, 
	void* key, 
	void* value, 
	void** out);

// tree_remove()
//
// Remove the value associated with `key`.
//
// In the event that the given key is a valid key
// into the tree, this function destroys the value
// associated with `key` using the user-provided 
// delete function and removes the key from the tree.
//
// NOTE: The semantics of this map are such that it
// will never try to destroy a key type that it maintains.
// This is to account for the most common cases in which
// types such as integral types or strings (static) are
// used as keys into the map. 
//
// Arguments:
//  tree - pointer to existing tree data structure
//  key  - the key for the value to remove
//
// Returns:
//  `true` on successful removal of the value associated with `key`
//  `false` otherwise
bool tree_remove(binary_tree_t* tree, void* key);

// tree_find()
//
// Search the tree for `key`, returning associated value if present.
//
// Arguments:
//	tree - pointer to existing tree data structure
//  key  - the key associated with the value for which to search
//
// Returns:
//  A pointer to the value associated with `key` if found
//  NULL otherwise
void *tree_find(binary_tree_t* tree, void* key);

// tree_count()
//
// Returns the current count of items in the tree.
//
// Arguments:
//	tree - pointer to existing tree data structure
//
// Returns:
//	The current count of items in the tree.
//  0 on invalid input
size_t tree_count(binary_tree_t* tree);

// tree_for_each()
//
// Walk the tree in pre-order traversal and invoke
// `iter` on each key / value pair.
//
// Arguments:
//	tree - pointer to existing tree data structure
//  iter - user-provided callback to be invoked on each key / value pair
void tree_for_each(binary_tree_t* tree, iterator_f iter);

#endif // BINARY_TREE_H 

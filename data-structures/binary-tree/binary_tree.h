// binary_tree.h
// Generic binary search tree data structure.

#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stddef.h>
#include <stdbool.h>

// Background:
//
// A binary tree is a node-based data structure that,
// as the name suggests, has properties that make it
// appear similar to a tree (i.e. the ones outside).
// More imporantly, however, is the fact that trees are
// fundamentally imporant data structures in programming
// and computer science because of their performance 
// properties - namely that, in the expected case, 
// operations on the tree (e.g. insert, find remove)
// take time that scales with the logarithm of the 
// number of items in the tree (that is O(log(n))).
// Compare this with data stuctures such as linked-lists
// and arrays where such operations might take time
// that scales linearly with the number of items
// (which is O(n)). This may not seem like a huge 
// improvement until you look at a graph of linear growth
// verus logarithmic growth for large values of n. 
// The benefits should be clear, and hopefully that
// is sufficiently motivating.
//
// Each node in the tree tracks three relationships:
// 
//	- Parent: the node appearing "above" it in the tree
//	- Left Child: one node appearing "below" it in the tree
//  - Right Child: one node appearing "below" it in the tree
//
// The "binary" part of the binary tree's name comes from the
// fact that it only maintains two child relationships. One 
// might imagine other tree designs wherein the tree might 
// maintain an arbitrary number of children, but here we stick
// with two for the binary tree.
// 
// The tree itself is composed of a root node which forms
// the base of the tree, and from there all relationships 
// are managed by individual nodes.
//
// In this module, we implement a specific type of binary tree
// called a "binary search tree" (BST). A BST has all the
// properties of a binary tree, plus some additional requirements:
//
// - The value associated with every node in the left subtree 
//   of a node in a binary search tree is LESS than the value 
//   associated with the node itself
// - Similarly, the value associated with every node in the right 
//   subtree of a node in a binary search tree is GREATER than 
//   the value associated with the node itself  
// - Finally, both the left and right subtrees of every node in a
//   binary search tree are themselves binary search trees (they
//   also meet the above requirements)
//
// Binary trees, and binary search trees in particular, are perfect
// candidates for the application of recursive algorithms in order
// to implement necessary operations. That is, in order to implement
// an effective operation a binary search tree, work out how you might
// express the operation in terms of individual nodes and the relationships
// they have with the two subtrees below them. That way, you can apply a
// base case (of sorts) to the current node, and, if necessary, recurse
// on the left and right subtrees of the current node to effectively
// implement an algorithm on an entire tree.

// The ordering type returned by user-provided comparator.
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

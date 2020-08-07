// spanning_tree.h
// A representation of the spanning tree constructed during BFS.

#ifndef SPANNING_TREE_H
#define SPANNING_TREE_H

#include "graph.h"

#include <stdbool.h>

typedef struct spanning_tree spanning_tree_t;

// The signature for the user-provided iterator function.
typedef void (*tree_iter_f)(
    vertex_id_t, 
    vertex_id_t, 
    size_t, 
    void*);

// spanning_tree_new()
// 
// Construct a new spanning tree.
//
// Returns:
//  A pointer to a newly constructed spanning tree object on success
//  NULL on failure (allocation failure)
spanning_tree_t* spanning_tree_new();

// spanning_tree_delete()
//
// Destroy an existing spanning tree object.
//
// Arguments:
//  tree - spanning tree object
void spanning_tree_delete(spanning_tree_t* tree);

// spanning_tree_add()
//
// Add a new vertex to the spanning tree.
//
// Arguments:
//  tree        - spanning tree object
//  id          - the identifier for the vertex 
//  path_length - the length of the path from the vertex to the root
//
// Returns:
//  `true` if the new vertex is successfully added
//  `false` otherwise
bool spanning_tree_add(
    spanning_tree_t* tree, 
    vertex_id_t      id, 
    vertex_id_t      backpointer,
    size_t           path_length);

// spanning_tree_count()
//
// Query the number of vertices spanned by the tree.
//
// Arguments:
//  tree - spanning tree object
//
// Returns:
//  The number of vertices spanned by the tree
//  0 on failure (invalid argument)
size_t spanning_tree_count(spanning_tree_t* tree);

// spanning_tree_for_each()
//
// Invoke the provided callback on each vertex in the tree.
//
// Arguments:
//  tree - spanning tree object
//  iter - the iterator invoked on each vertex in the tree
//  ctx  - arbitrary user-provided context passed to callback
void spanning_tree_for_each(
    spanning_tree_t* tree, 
    tree_iter_f      iter, 
    void*            ctx);

#endif // SPANNING_TREE_H
// dijkstra.c
// Computing single-source shortest path on 
// a weighted graph with Dijkstra's algorithm.

#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"

// Sentinel value to denote length for invalid path.
#define NULL_PATH_LENGTH -1

// Sentinel value to denote weight for invalid path.
#define NULL_PATH_WEIGHT -1

// The path returned by the search.
typedef struct path path_t;

// The signature for the path iterator callback.
typedef void (*path_iter_f)(vertex_id_t, void* ctx);

// dijkstra()
//
// Compute the minimal weight path in graph `g`
// from source vertex `src` to destination vertex `dst`
// using dijkstra's graph search algorithm.
//
// Arguments:
//  graph - the graph in which to perform the search
//  src   - identifier for the source vertex
//  dst   - identifier for the destination vertex
//
// Returns:
//  A pointer to a path object to represent the result of the search
//  NULL in the event the search failed (dst not reachable from src)
path_t* dijkstra(
    graph_t*    graph, 
    vertex_id_t src, 
    vertex_id_t dst);

// path_delete()
//
// Destroy a path object returned by dijkstra's algorithm.
//
// Arguments:
//  path - a path object
void path_delete(path_t* path);

// path_length()
//
// Query the length of the path returned by dijkstra's algorithm.
//
// Arguments:
//  path - a path object
//
// Returns:
//  The length of the provided path object
//  NULL_PATH_LENGTH on invalid path
ssize_t path_length(path_t* path);

// path_weight()
//
// Query the weight of the path returned by dijkstra's algorithm.
//
// Arguments:
//  path - a path object
//
// Returns:
//  The weight of the provided path object
//  NULL_PATH_WEIGHT on invalid path
ssize_t path_weight(path_t* path);

// path_for_each()
//
// Invoke the provided iterator function on each
// vertex in the provided path object.
//
// Arguments:
//  path - a path object
//  iter - the iterator callback function
//  ctx  - arbitrary user-provided context
void path_for_each(
    path_t*     path, 
    path_iter_f iter, 
    void*       ctx);

#endif // DIJKSTRA_H
// bfs.h
// Using the breadth-first graph search algorithm
// to construct a spanning tree of a graph.

#ifndef BFS_H
#define BFS_H

#include "graph.h"
#include "spanning_tree.h"

// breadth_first_search()
//
// Construct a spanning tree of the graph `graph` from the 
// root vertex `root` using the breadth-first search algorithm.
//
// Arguments:
//  graph - the source graph
//  root  - the identifier for the root vertex
//
// Returns:
//  A pointer to a spanning tree object constructed by the search
//  NULL on algorithm failure
spanning_tree_t* breadth_first_search(
    graph_t*    graph, 
    vertex_id_t root_id);

#endif // BFS_H
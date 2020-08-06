// graph.h
// Weighted graph data structure.

#ifndef GRAPH_H
#define GRAPH_H

#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>

typedef struct graph graph_t;

typedef ssize_t vertex_id_t;

// Sentinel to denote failed vertex insertion.
#define NULL_VERTEX_ID -1

// Sentinel value to indicate failed edge query. 
#define NULL_EDGE_WEIGHT -1

// Sentinel value to indicatet failed vertex count query.
#define NULL_VERTEX_COUNT -1

// Sentinel value to indicate failed edge count query.
#define NULL_EDGE_COUNT -1

// Sentinel value to indicate failed vertex degree query.
#define NULL_VERTEX_DEGREE -1

// The signature for the user-provided iterator function.
typedef void (*graph_iter_f)(vertex_id_t, ssize_t, void*);

// graph_new()
//
// Construct a new graph.
//
// Returns:
//  A pointer to a newly constructed graph on success
//  NULL on failure
graph_t* graph_new();

// graph_delete()
//
// Destroy and existing graph.
//
// Arguments:
//  graph - pointer to an existing graph
void graph_delete(graph_t* graph);

// graph_vertex_count()
//
// Return the number of vertices in the graph.
//
// Arguments:
//  graph - pointer to an existing graph
//
// Returns:
//  The number of verticies in the graph
//  NULL_VERTEX_COUNT on failure (invalid argument)
ssize_t graph_vertex_count(graph_t* graph);

// graph_edge_count()
//
// Return the number of edge in the graph.
//
// Arguments:
//  graph - pointer to an existing graph
//
// Returns:
//  The number of edgew in the graph
//  NULL_EDGE_COUNT on failure (invalid argument)
ssize_t graph_edge_count(graph_t* graph);

// graph_add_vertex()
//
// Add a new vertex to the graph.
//
// Arguments:
//  graph - pointer to an existing graph
//
// Returns:
//  A vertex identifier for the vertex added to the graph
//  NULL_VERTEX_ID on failure (allocation failure)
vertex_id_t graph_add_vertex(graph_t* graph);

// graph_remove_vertex()
//
// Remove the vertex identified by the 
// given vertex ID from the graph.
//
// Arguments:
//  graph - pointer to an existing graph
//  v_id  - the vertex ID of the vertex to remove 
//
// Returns:
//  `true` if the vertex is successfully removed
//  `false` otherwise
bool graph_remove_vertex(
    graph_t*    graph, 
    vertex_id_t v_id);

// graph_get_vertex_degree()
//
// Return the degree of the vertex identified by `v_id`.
//
// Arguments:
//  graph - pointer to an existing graph
//  v_id  - the vertex ID of the vertex to query
//
// Returns:
//  The degree of the vertex identified by `v_id`
//  NULL_VERTEX_DEGREE on failure (invalid vertex ID)
ssize_t graph_get_vertex_degree(
    graph_t*    graph, 
    vertex_id_t v_id);

// graph_add_edge()
//
// Add an edge between the verticies identified
// by `v1_id` and `v2_id` with weight `weight`
// to the graph.
//
// NOTE: edge weights must be nonzero.
//
// Arguments:
//  graph  - pointer to an existing graph
//  v1_id  - identifier for vertex 
//  v2_id  - identifier for vertex
//  weight - the weight to associate with the new edge
//
// Returns:
//  `true` if the edge is successfully added, or edge already exists
//  `false` otherwise (invlid vertex ID(s), allocation failure)
bool graph_add_edge(
    graph_t*    graph, 
    vertex_id_t v1_id, 
    vertex_id_t v2_id,
    size_t      weight);

// graph_remove_edge()
//
// Remove the edge between the verticies identified
// by `v1_id` and `v2_id` from the graph.
//
// Arguments:
//  graph - pointer to an existing graph
//  v1_id - identifier for vertex 
//  v2_id - identifier for vertex
//
// Returns:
//  `true` if the edge is successfully removed from the graph
//  `false` otherwise (invalid vertex ID(s))
bool graph_remove_edge(
    graph_t*    graph, 
    vertex_id_t v1_id, 
    vertex_id_t v2_id);

// graph_has_edge()
//
// Determine if the graph has an edge between the
// verticies identified by `v1_id` and `v2_id`.
//
// Arguments:
//  graph - pointer to an existing graph
//  v1_id - identifier for vertex 
//  v2_id - identifier for vertex
// 
// Returns:
//  `true` if the graph contains the specified edge
//  `false` otherwise
bool graph_has_edge(
    graph_t*    graph, 
    vertex_id_t v1_id, 
    vertex_id_t v2_id);

// graph_get_edge_weight()
//
// Determine the weight of the edge between the
// vertices identified by `v1_id` and `v2_id`.
//
// Arguments:
//  graph - pointer to an existing graph
//  v1_id - identifier for vertex 
//  v2_id - identifier for vertex
//
// Returns:
//  The weight of the edge between verticies identified by `v1_id` and `v2_id`
//  NULL_EDGE_WEIGHT on failure (invalid vertex ID(s))
ssize_t graph_get_edge_weight(
    graph_t*    graph,
    vertex_id_t v1_id,
    vertex_id_t v2_id);

// graph_for_each_adjacent()
//
// Invoke the given iterator function on each
// vertex adjacent to the vertex identified by `v_id`.
//
// Arguments:
//  graph    - pointer to an existing graph
//  v_id     - the identifier for the vertex 
//  iterator - the function to invoke on each adjacent vertex
//  ctx      - arbitrary user context passed to callback
void graph_for_each_adjacent(
    graph_t*     graph, 
    vertex_id_t  v_id, 
    graph_iter_f iterator, 
    void*        ctx);

#endif // GRAPH_H
// graph.c
// Weighted graph data structure.

#include "graph.h"

#include <stdlib.h>

// ----------------------------------------------------------------------------
// Exported

graph_t* graph_new()
{
    return NULL;
}

void graph_delete(graph_t* graph)
{
    return;
}

ssize_t graph_vertex_count(graph_t* graph)
{
    return NULL_VERTEX_COUNT;
}

ssize_t graph_edge_count(graph_t* graph)
{
    return NULL_EDGE_COUNT;
}

vertex_id_t graph_add_vertex(graph_t* graph)
{
    return NULL_VERTEX_ID;
}

bool graph_remove_vertex(
    graph_t*    graph, 
    vertex_id_t v_id)
{
    return false;
}

ssize_t graph_get_vertex_degree(
    graph_t*    graph, 
    vertex_id_t v_id)
{
    return NULL_VERTEX_DEGREE;
}

bool graph_add_edge(
    graph_t*    graph, 
    vertex_id_t v1_id, 
    vertex_id_t v2_id,
    size_t      weight)
{
    return false;
}

bool graph_remove_edge(
    graph_t*    graph, 
    vertex_id_t v1_id, 
    vertex_id_t v2_id)
{
    return false;
}

bool graph_has_edge(
    graph_t*    graph, 
    vertex_id_t v1_id, 
    vertex_id_t v2_id)
{
    return false;
}

ssize_t graph_get_edge_weight(
    graph_t*    graph,
    vertex_id_t v1_id,
    vertex_id_t v2_id)
{
    return NULL_EDGE_WEIGHT;
}

void graph_for_each_adjacent(
    graph_t*     graph, 
    vertex_id_t  v_id, 
    graph_iter_f iterator, 
    void*        ctx)
{
    return;
}
// dijkstra.c
// Computing single-source shortest path on 
// a weighted graph with Dijkstra's algorithm.

#include "dijkstra.h"

path_t* dijkstra(
    graph_t*    graph, 
    vertex_id_t src, 
    vertex_id_t dst)
{
    return NULL;
}

void path_delete(path_t* path)
{
    return;
}

ssize_t path_length(path_t* path)
{
    return NULL_PATH_LENGTH;
}

ssize_t path_weight(path_t* path)
{
    return NULL_PATH_WEIGHT;
}

void path_for_each(
    path_t*     path, 
    path_iter_f iter, 
    void*       ctx)
{
    return;
}

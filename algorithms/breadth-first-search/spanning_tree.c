// spanning_tree.c
// A representation of the spanning tree constructed during BFS.

#include "spanning_tree.h"

#include <stdlib.h>

spanning_tree_t* spanning_tree_new()
{
    return NULL;
}

void spanning_tree_delete(spanning_tree_t* tree)
{
    return;
}

bool spanning_tree_add(
    spanning_tree_t* tree, 
    vertex_id_t      id, 
    vertex_id_t      backpointer,
    size_t           path_length)
{
    return false;
}

size_t spanning_tree_count(spanning_tree_t* tree)
{
    return 0;
}

void spanning_tree_for_each(
    spanning_tree_t* tree, 
    tree_iter_f      iter, 
    void*            ctx)
{
    return;
}
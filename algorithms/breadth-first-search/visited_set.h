// visited_set.h
// A non-generic set for tracking visited verticies in breadth-first search.

#ifndef VISITED_SET_H
#define VISITED_SET_H

#include "graph.h"

#include <stdbool.h>

typedef struct visited_entry visited_entry_t;

typedef struct visited_set visited_set_t;

visited_set_t* set_new();

void set_delete(visited_set_t* set);

void set_add(
    visited_set_t* set, 
    vertex_id_t    id,
    vertex_id_t    backpointer,
    ssize_t        weight);

bool set_contains(
    visited_set_t* set, 
    vertex_id_t    id);

ssize_t set_backpointer_for_id(
    visited_set_t* set, 
    vertex_id_t    id);

ssize_t set_weight_for_id(
    visited_set_t* set, 
    vertex_id_t    id);

#endif // VISITED_SET_H
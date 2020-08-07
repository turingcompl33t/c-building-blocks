// graph.c

#include "graph.h"
#include "intrusive_list.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

// ----------------------------------------------------------------------------
// Internal Declarations

// An entry in the intrusive list of adjacent veritices.
typedef struct adjacent_entry
{
    // Entry in intrusive list of adjacent vertices.
    list_entry_t entry;

    // The vertex identified by this entry.
    vertex_id_t vertex_id;

    // The weight of the edge between the vertex in whose
    // adjacency list this vertex appears and this vertex.
    size_t weight;
} adjacent_entry_t;

// An entry in the intrusive list of vertices.
typedef struct vertex_entry
{
    // Entry in intrusive linked-list of vertices.
    list_entry_t entry;

    // Head of intrusive list of adjacent vertices.
    list_entry_t adjacent;

    // The vertex identified by this entry.
    vertex_id_t vertex_id;
} vertex_entry_t;

struct graph
{   
    // The intrusive linked-list of vertices.
    list_entry_t vertices;

    // The current number of vertices in the graph.
    ssize_t n_vertices;

    // The current number of edges in the graph.
    ssize_t n_edges;

    // The next vertex identifier to be handed out.
    ssize_t vertex_id;
};

static bool add_adjacent(
    vertex_entry_t* vertex_entry, 
    vertex_id_t     v_id, 
    size_t          weight);
static void remove_adjacent(
    vertex_entry_t* vertex_entry, 
    vertex_id_t     v_id);
static adjacent_entry_t* find_adjacent(
    vertex_entry_t* vertex_entry, 
    vertex_id_t     v_id);
static bool has_adjacent(
    vertex_entry_t* vertex_entry, 
    vertex_id_t     v_id);

static vertex_entry_t* find_vertex(
    list_entry_t* vertices, 
    vertex_id_t   v_id);
static bool find_vertex_by_id(list_entry_t* entry, void* ctx);
static bool find_adjacent_vertex_by_id(list_entry_t* entry, void* ctx);

static vertex_entry_t* new_vertex_entry(vertex_id_t v_id);
static void delete_vertex_entry(vertex_entry_t* vertex_entry);

static adjacent_entry_t* new_adjacent_entry(vertex_id_t v_id, size_t weight);
static void delete_adjacent_entry(adjacent_entry_t* adjacent_entry);

static void flush_vertices(graph_t* graph);
static void flush_adjacency_list(vertex_entry_t* vertex_entry);

// ----------------------------------------------------------------------------
// Exported

graph_t* graph_new()
{
    graph_t* graph = malloc(sizeof(graph_t));
    if (NULL == graph)
    {
        return NULL;
    }

    list_init(&graph->vertices);

    graph->n_vertices = 0;
    graph->n_edges    = 0;

    graph->vertex_id  = 0;

    return graph;
}

void graph_delete(graph_t* graph)
{
    if (NULL == graph)
    {
        return;
    }

    flush_vertices(graph);
    free(graph);
}

ssize_t graph_vertex_count(graph_t* graph)
{
    return (NULL == graph) ? NULL_VERTEX_COUNT : graph->n_vertices;
}

ssize_t graph_edge_count(graph_t* graph)
{
    return (NULL == graph) ? NULL_EDGE_COUNT : graph->n_edges;
}

vertex_id_t graph_add_vertex(graph_t* graph)
{
    if (NULL == graph)
    {
        return NULL_VERTEX_ID;
    }

    vertex_entry_t* new_entry = new_vertex_entry(graph->vertex_id);
    if (NULL == new_entry)
    {
        return NULL_VERTEX_ID;
    }

    list_push_back(
        &graph->vertices, 
        &new_entry->entry);

    graph->n_vertices++;
    return graph->vertex_id++;
}

bool graph_remove_vertex(
    graph_t*    graph, 
    vertex_id_t v_id)
{
    if (NULL == graph || 0 == graph->n_vertices)
    {
        return false;
    }

    vertex_entry_t* target = find_vertex(&graph->vertices, v_id);
    if (NULL == target)
    {
        return false;
    }

    adjacent_entry_t* adjacent_entry;
    while ((adjacent_entry 
        = (adjacent_entry_t*) list_pop_front(&target->adjacent)) != NULL)
    {
        vertex_entry_t* vertex_for_adjacent 
            = find_vertex(&graph->vertices, adjacent_entry->vertex_id);

        // logic error
        assert(vertex_for_adjacent != NULL);

        remove_adjacent(vertex_for_adjacent, v_id);

        delete_adjacent_entry(adjacent_entry);

        graph->n_edges--;
    }

    list_remove_entry(&graph->vertices, &target->entry);
    delete_vertex_entry(target);

    graph->n_vertices--;

    return true;
}

ssize_t graph_get_vertex_degree(
    graph_t*    graph, 
    vertex_id_t v_id)
{
    if (NULL == graph)
    {
        return NULL_VERTEX_DEGREE;
    }

    vertex_entry_t* vertex_entry = find_vertex(&graph->vertices, v_id);

    return (NULL == vertex_entry) 
        ? NULL_VERTEX_DEGREE
        : list_count(&vertex_entry->adjacent);
}

bool graph_add_edge(
    graph_t*    graph, 
    vertex_id_t v1_id, 
    vertex_id_t v2_id,
    size_t      weight)
{
    if ((NULL == graph) 
     || (NULL_EDGE_WEIGHT == weight)
     || (v1_id == v2_id))
    {
        return false;
    }

    vertex_entry_t* v1_entry = find_vertex(&graph->vertices, v1_id);
    vertex_entry_t* v2_entry = find_vertex(&graph->vertices, v2_id);

    if (NULL == v1_entry || NULL == v2_entry)
    {
        return false;
    }

    // ensure that we do not add an edge if it already exists
    if (graph_has_edge(graph, v1_id, v2_id))
    {
        return true;
    }

    assert(add_adjacent(v1_entry, v2_id, weight));
    assert(add_adjacent(v2_entry, v1_id, weight));

    graph->n_edges++;

    return true;
}

bool graph_remove_edge(
    graph_t*    graph, 
    vertex_id_t v1_id, 
    vertex_id_t v2_id)
{
    if ((NULL == graph) || (v1_id == v2_id))
    {
        return false;
    }

    vertex_entry_t* v1_entry = find_vertex(&graph->vertices, v1_id);
    vertex_entry_t* v2_entry = find_vertex(&graph->vertices, v2_id);

    if ((NULL == v1_entry) || (NULL == v2_entry))
    {
        return false;
    }

    remove_adjacent(v1_entry, v2_id);
    remove_adjacent(v2_entry, v1_id);

    graph->n_edges--;

    return true;
}

bool graph_has_edge(
    graph_t*    graph, 
    vertex_id_t v1_id, 
    vertex_id_t v2_id)
{
    if (NULL == graph)
    {
        return false;
    }

    vertex_entry_t* v1_entry = find_vertex(&graph->vertices, v1_id);
    vertex_entry_t* v2_entry = find_vertex(&graph->vertices, v2_id);

    if (NULL == v1_entry || NULL == v2_entry)
    {
        return false;
    }

    // NOTE: no need to perform a lookup on both
    // adjacency lists, but this is a good opportunity 
    // to check for logic errors in the implementation

    const bool v1_result = has_adjacent(v1_entry, v2_id);
    const bool v2_result = has_adjacent(v2_entry, v1_id);

    assert((v1_result && v2_result) || (!v1_result && !v2_result));

    return (v1_result && v2_result);
}

ssize_t graph_get_edge_weight(
    graph_t*    graph,
    vertex_id_t v1_id,
    vertex_id_t v2_id)
{
    if (NULL == graph)
    {
        return NULL_EDGE_WEIGHT;
    }

    vertex_entry_t* v1_entry = find_vertex(&graph->vertices, v1_id);
    vertex_entry_t* v2_entry = find_vertex(&graph->vertices, v2_id);

    if (NULL == v1_entry || NULL == v2_entry)
    {
        return NULL_EDGE_WEIGHT;
    }

    // NOTE: no need to perform a lookup on both
    // adjacency lists, but this is a good opportunity 
    // to check for logic errors in the implementation

    adjacent_entry_t* v1_to_v2 = find_adjacent(v1_entry, v2_id);
    adjacent_entry_t* v2_to_v1 = find_adjacent(v2_entry, v1_id);

    assert(((NULL == v1_to_v2) && (NULL == v2_to_v1)) 
        || ((v1_to_v2 != NULL) && (v2_to_v1 != NULL)));

    assert(v1_to_v2->weight == v2_to_v1->weight);

    return v1_to_v2->weight;
}

void graph_for_each_adjacent(
    graph_t*     graph, 
    vertex_id_t  v_id, 
    graph_iter_f iterator, 
    void*        ctx)
{
    if (NULL == graph)
    {
        return;
    }

    vertex_entry_t* vertex_entry = find_vertex(&graph->vertices, v_id);
    if (NULL == vertex_entry)
    {
        return;
    }
    
    // NOTE: not particularly happy with this solution;
    // essentially had to re-implement list_for_each()
    // because the type of the iterator callback is distinct

    list_entry_t* head = &vertex_entry->adjacent;
    list_entry_t* current;
    for (current = head->flink;
         current != head; 
         current = current->flink)
    {
        adjacent_entry_t* as_adjacent = (adjacent_entry_t*) current;
        iterator(as_adjacent->vertex_id, as_adjacent->weight, ctx);
    }
}

// ----------------------------------------------------------------------------
// Internal

static bool add_adjacent(
    vertex_entry_t* vertex_entry, 
    vertex_id_t     v_id, 
    size_t          weight)
{
    adjacent_entry_t* adjacent_entry = new_adjacent_entry(v_id, weight);
    if (NULL == adjacent_entry)
    {
        return false;
    }

    list_push_front(&vertex_entry->adjacent, &adjacent_entry->entry);

    return true;
}

static void remove_adjacent(
    vertex_entry_t* vertex_entry, 
    vertex_id_t     v_id)
{
    adjacent_entry_t* target 
        = (adjacent_entry_t*) list_find(&vertex_entry->adjacent, find_adjacent_vertex_by_id, &v_id);

    // logic error
    assert(target != NULL);

    list_remove_entry(&vertex_entry->adjacent, &target->entry);

    delete_adjacent_entry(target);
}

static adjacent_entry_t* find_adjacent(
    vertex_entry_t* vertex_entry, 
    vertex_id_t     v_id)
{
    return (adjacent_entry_t*) list_find(&vertex_entry->adjacent, find_adjacent_vertex_by_id, &v_id);
}

static bool has_adjacent(
    vertex_entry_t* vertex_entry, 
    vertex_id_t     v_id)
{
    adjacent_entry_t* adj = (adjacent_entry_t*)list_find(&vertex_entry->adjacent, find_adjacent_vertex_by_id, &v_id);
    return adj != NULL;
}

static vertex_entry_t* find_vertex(
    list_entry_t* vertices, 
    vertex_id_t   v_id)
{
    vertex_entry_t* vertex_entry = (vertex_entry_t*)list_find(
        vertices, 
        find_vertex_by_id,
        &v_id);

    return vertex_entry;
}

static bool find_vertex_by_id(list_entry_t* entry, void* ctx)
{
    vertex_entry_t* as_vertex_entry = (vertex_entry_t*) entry;
    vertex_id_t* as_vid = (vertex_id_t*) ctx;

    return as_vertex_entry->vertex_id == *as_vid;
}

static bool find_adjacent_vertex_by_id(list_entry_t* entry, void* ctx)
{
    adjacent_entry_t* as_adjacent_entry = (adjacent_entry_t*) entry;
    vertex_id_t* as_vid = (vertex_id_t*) ctx;

    return as_adjacent_entry->vertex_id == *as_vid;
}

static vertex_entry_t* new_vertex_entry(vertex_id_t v_id)
{
    vertex_entry_t* vertex_entry = malloc(sizeof(vertex_entry_t));
    if (NULL == vertex_entry)
    {
        return NULL;
    }

    vertex_entry->vertex_id = v_id;
    list_init(&vertex_entry->adjacent);

    return vertex_entry;
}

static void delete_vertex_entry(vertex_entry_t* vertex_entry)
{
    free(vertex_entry);
}

static adjacent_entry_t* new_adjacent_entry(vertex_id_t v_id, size_t weight)
{
    adjacent_entry_t* adjacent_entry = malloc(sizeof(adjacent_entry_t));
    if (NULL == adjacent_entry)
    {
        return NULL;
    }

    adjacent_entry->vertex_id = v_id;
    adjacent_entry->weight    = weight;

    return adjacent_entry;
}

static void delete_adjacent_entry(adjacent_entry_t* adjacent_entry)
{
    free(adjacent_entry);
}

static void flush_vertices(graph_t* graph)
{
    vertex_entry_t* current;
    while ((current 
        = (vertex_entry_t*)list_pop_front(&graph->vertices)) != NULL)
    {
        flush_adjacency_list(current);
        delete_vertex_entry(current);
    }
}

static void flush_adjacency_list(vertex_entry_t* vertex_entry)
{
    adjacent_entry_t* current;
    while ((current = 
        (adjacent_entry_t*)list_pop_front(&vertex_entry->adjacent)) != NULL)
    {
        delete_adjacent_entry(current);
    }
}
// bfs.c
// Using the breadth-first graph search algorithm
// to construct a spanning tree of a graph.

#include "bfs.h"
#include "graph.h"
#include "queue.h"
#include "visited_set.h"
#include "spanning_tree.h"

#include <stdlib.h>

// ----------------------------------------------------------------------------
// Internal Declarations

// A search item that is processed via the queue.
typedef struct search_item
{
    vertex_id_t id;
    size_t      path_length;
} search_item_t;

typedef struct adjacent_iterator_ctx
{
    queue_t*       search_queue;
    visited_set_t* visited;
    vertex_id_t    backpointer;
    size_t         path_length;
} adjacent_iterator_ctx_t;

static spanning_tree_t* construct_spanning_tree(
    visited_set_t* visited);
static void visited_set_iterator(
    vertex_id_t id, 
    vertex_id_t backpointer, 
    ssize_t     weight,
    void*       ctx);

static void adjacent_vertex_iterator(
    vertex_id_t id, 
    ssize_t     weight, 
    void*       ctx);

static search_item_t* new_search_item(
    vertex_id_t id, 
    size_t      path_length);
static void delete_search_item(search_item_t* item);
static void search_item_deleter(void* item);

// ----------------------------------------------------------------------------
// Exported

spanning_tree_t* breadth_first_search(
    graph_t*    graph, 
    vertex_id_t root_id)
{
    // construct a queue 
    queue_t* search_queue = queue_new(search_item_deleter);

    // construct a set to maintain visisted vertices
    visited_set_t* visited = set_new();

    // make a new search item for the root node
    search_item_t* root = new_search_item(root_id, 0);
    queue_push(search_queue, root);

    // and mark the root as discovered
    set_add(visited, root_id, NULL_VERTEX_ID, 0);

    adjacent_iterator_ctx_t iterator_ctx = {
        .search_queue = search_queue,
        .visited      = visited,
        .backpointer  = NULL_VERTEX_ID,
        .path_length  = 0
    };

    // perform the search
    search_item_t* si;
    while ((si = queue_pop(search_queue)) != NULL)
    {
        iterator_ctx.backpointer = si->id;
        iterator_ctx.path_length = si->path_length;

        graph_for_each_adjacent(
            graph, 
            si->id, 
            adjacent_vertex_iterator, 
            &iterator_ctx);

        delete_search_item(si);
    } 

    // construct the spanning tree from the visited set
    spanning_tree_t* tree = construct_spanning_tree(visited);

    // cleanup
    set_delete(visited);
    queue_delete(search_queue);

    return tree;
}

// ----------------------------------------------------------------------------
// Internal

static spanning_tree_t* construct_spanning_tree(
    visited_set_t* visited)
{
    spanning_tree_t* tree = spanning_tree_new();
    
    set_for_each(visited, visited_set_iterator, tree);

    return tree;
}

static void visited_set_iterator(
    vertex_id_t id, 
    vertex_id_t backpointer, 
    ssize_t     weight,
    void*       ctx)
{
    spanning_tree_t* tree = (spanning_tree_t*) ctx;
    spanning_tree_add(tree, id, backpointer, weight);
}

static void adjacent_vertex_iterator(
    vertex_id_t id, 
    ssize_t     weight, 
    void*       ctx)
{
    adjacent_iterator_ctx_t* adj_ctx = (adjacent_iterator_ctx_t*) ctx;

    queue_t* search_queue  = adj_ctx->search_queue;
    visited_set_t* visited = adj_ctx->visited;

    if (!set_contains(visited, id))
    {
        // this vertex has not yet been explored

        // the path length required to reach this vertex is the
        // length of the path required to reach the adjacent
        // vertex from which it is reached, plus 1 for the adjoining edge
        const size_t path_length = adj_ctx->path_length + 1;

        // mark the vertex as visited
        set_add(visited, id, adj_ctx->backpointer, path_length);

        // and add vertex to the search queue to be explored later
        search_item_t* item = new_search_item(id, path_length);
        queue_push(search_queue, item);
    }
}

static search_item_t* new_search_item(
    vertex_id_t id, 
    size_t      path_length)
{
    search_item_t* item = malloc(sizeof(search_item_t));
    if (NULL == item)
    {
        return NULL;
    }

    item->id          = id;
    item->path_length = path_length;

    return item;
}

static void delete_search_item(search_item_t* item)
{
    free(item);
}

static void search_item_deleter(void* item)
{
    search_item_t* as_item = (search_item_t*) item;
    free(as_item);
}
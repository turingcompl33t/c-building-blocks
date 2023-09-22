// dijkstra.c
// Computing single-source shortest path on 
// a weighted graph with Dijkstra's algorithm.

#include "graph.h"
#include "dijkstra.h"
#include "visited_set.h"
#include "priority_queue.h"

#include <assert.h>
#include <stdlib.h>

// ----------------------------------------------------------------------------
// Internal Declarations

typedef struct path_entry
{
    struct path_entry* next;
    vertex_id_t        id;
} path_entry_t;

struct path
{
    // The head of the linked-list of path items.
    path_entry_t* head;

    // The total length of the path.
    size_t length;

    // The total weight of the path.
    ssize_t weight;
};

// A pair of a vertex identifier and the 
// cumulative weight of the path to reach it.
typedef struct weighted_vertex
{
    vertex_id_t id;
    vertex_id_t backpointer;
    ssize_t     weight;
} weighted_vertex_t;

// The context passed to the for_each_adjacent()
// callback in order to process adjacent vertices.
typedef struct adjacent_iterator_ctx
{
    // A reference to the visited set.
    visited_set_t* visited_set;

    // A reference to the priority queue of weighted vertices.
    priority_queue_t* queue;

    // The vertex that is currently being processed in
    // each iteration of the loop serves as the backpointer
    // for each of its adjacent vertices in the callback
    vertex_id_t backpointer;

    // The weight of lowest-weight path to the vertex
    // for which neighbors are currently being processed.
    ssize_t path_weight;
} adjacent_iterator_ctx_t;

static void adjacent_vertex_iterator(
    vertex_id_t id, 
    ssize_t     weight, 
    void*       ctx);

static priority_t prioritize_by_lowest_weight(void* wv1, void* wv2);
static void weighted_vertex_deleter(void* wv);

static weighted_vertex_t* new_weighted_vertex(
    vertex_id_t id, 
    vertex_id_t backpointer,
    ssize_t     weight);
static void delete_weighted_vertex(weighted_vertex_t* wv);

path_t* backtrace(
    visited_set_t* visited_set, 
    vertex_id_t    src, 
    vertex_id_t    dst);

path_t* path_new();
void path_push(path_t* path, vertex_id_t id);

path_entry_t* path_entry_new(vertex_id_t id);
void path_entry_delete(path_entry_t* entry);

// ----------------------------------------------------------------------------
// Exported

path_t* dijkstra(
    graph_t*    graph, 
    vertex_id_t src, 
    vertex_id_t dst)
{
    // construct a priority queue for weighted vertices
    priority_queue_t* queue = queue_new(
        prioritize_by_lowest_weight,
        weighted_vertex_deleter);

    // construct the visited set
    visited_set_t* visited_set = set_new();

    // construct the iterator context
    adjacent_iterator_ctx_t iterator_ctx = {
        .queue       = queue,
        .visited_set = visited_set,
        .backpointer = NULL_VERTEX_ID,
        .path_weight = 0
    };

    // create the root vertex; we use the reserved 
    // vertex ID to denote the root vertex backpointer
    weighted_vertex_t* root_vertex = new_weighted_vertex(src, NULL_VERTEX_ID, 0);

    // enqueue the root vertex
    queue_push(queue, root_vertex);

    // continue to iterate until all vertices are visited
    weighted_vertex_t* v;
    while ((v = queue_pop(queue)) != NULL)
    {
        if (set_contains(visited_set, v->id))
        {
            // this vertex was already visited along a lower-weight path;
            // nothing remains to be done to process this vertex
            delete_weighted_vertex(v);
            continue;
        }

        // now that v has been popped, we are assured that we
        // have found the lowest-weight path from the src vertex
        // to v (because we assume no negative weight egdes, 
        // perhaps we'll take a crack at Bellman-Ford in a future 
        // module to overcome this limitation) so we can safely 
        // add v to the visited set with current weight

        // mark the vertex as visited
        set_add(visited_set, v->id, v->backpointer, v->weight);

        if (v->id == dst)
        {
            // located the destination vertex, break early
            break;
        }

        // update the path weight in the iterator context
        iterator_ctx.backpointer = v->id;
        iterator_ctx.path_weight = v->weight;

        // process the adjacent vertices for the current vertex
        graph_for_each_adjacent(
            graph, 
            v->id, 
            adjacent_vertex_iterator, 
            &iterator_ctx);
    } 

    // backtrace to reconstruct the path
    path_t* path = backtrace(visited_set, src, dst);

    // cleanup
    set_delete(visited_set);
    queue_delete(queue);

    return path;
}

void path_delete(path_t* path)
{
    if (NULL == path)
    {
        return;
    }

    path_entry_t* current = path->head;
    while (current != NULL)
    {
        path_entry_t* tmp = current->next;
        path_entry_delete(current);
        current = tmp;
    }
}

ssize_t path_length(path_t* path)
{
    return (NULL == path)
        ? NULL_PATH_LENGTH
        : path->length;
}

ssize_t path_weight(path_t* path)
{
    return (NULL == path) 
        ? NULL_PATH_WEIGHT 
        : path->weight; 
}

void path_for_each(
    path_t*     path, 
    path_iter_f iter, 
    void*       ctx)
{
    if (NULL == path)
    {
        return;
    }

    path_entry_t* current;
    for (current = path->head; 
         current != NULL; 
         current = current->next)
    {
        iter(current->id, ctx);
    }
}

// ----------------------------------------------------------------------------
// Internal: BFS Support

static void adjacent_vertex_iterator(
    vertex_id_t id, 
    ssize_t     weight, 
    void*       ctx)
{
    adjacent_iterator_ctx_t* adj_ctx = (adjacent_iterator_ctx_t*)ctx;
    
    if (!set_contains(adj_ctx->visited_set, id))
    {
        // this vertex has not been added to the visited set;
        // this implies that this vertex does not yet have
        // a definitive lowest-weight path established, so
        // we need to add it to the queue to process it

        // the backpointer for the current vertex is the 
        // adjacent vertex via which we have arrived at this one
        const vertex_id_t backpointer = adj_ctx->backpointer;

        // the total path weight for the path to this vertex
        // under consideration is the weight of the (lowest-weight)
        // path to reach its adjacent vertex plus the weight of the
        // edge between the current vertex and its neighbor
        const ssize_t path_weight = adj_ctx->path_weight + weight;

        weighted_vertex_t* wv = new_weighted_vertex(
            id, 
            backpointer, 
            path_weight);

        queue_push(adj_ctx->queue, wv);
    }
}

static priority_t prioritize_by_lowest_weight(void* wv1, void* wv2)
{
    weighted_vertex_t* as_wv1 = (weighted_vertex_t*) wv1;
    weighted_vertex_t* as_wv2 = (weighted_vertex_t*) wv2;

    if (as_wv1->weight < as_wv2->weight)
    {
        return GREATER;
    }
    else if (as_wv1->weight > as_wv2->weight)
    {
        return LESS;
    }
    else
    {
        return EQUAL;
    }
}

static void weighted_vertex_deleter(void* wv)
{
    weighted_vertex_t* as_wv = (weighted_vertex_t*) wv;
    free(as_wv);
}

static weighted_vertex_t* new_weighted_vertex(
    vertex_id_t id, 
    vertex_id_t backpointer, 
    ssize_t     weight)
{
    weighted_vertex_t* wv = malloc(sizeof(weighted_vertex_t));
    
    // NOTE: obviously this error-handling leaves much to
    // be desired, but we all need to choose our battles
    assert(wv);

    wv->id          = id;
    wv->backpointer = backpointer;
    wv->weight      = weight;

    return wv;
}

static void delete_weighted_vertex(weighted_vertex_t* wv)
{
    free(wv);
}

// ----------------------------------------------------------------------------
// Internal: Path Support

path_t* backtrace(
    visited_set_t* visited_set, 
    vertex_id_t    src, 
    vertex_id_t    dst)
{
    if (!set_contains(visited_set, dst))
    {
        // the destination vertex was not reachable from the source
        return NULL;
    }

    path_t* path = path_new();

    vertex_id_t current = dst;
    while (current != src)
    {
        path_push(path, current);
        path->length++;

        current = set_backpointer_for_id(visited_set, current);
    }
    path_push(path, src);

    // the total weight of the path from source to destination
    path->weight = set_weight_for_id(visited_set, dst);

    return path;
}

path_t* path_new()
{
    path_t* path = malloc(sizeof(path_t));
    assert(path);

    path->head   = NULL;
    path->length = 0;
    path->weight = NULL_EDGE_WEIGHT;

    return path;
}

void path_push(path_t* path, vertex_id_t id)
{
    path_entry_t* entry = path_entry_new(id);

    entry->next = path->head;
    path->head  = entry;
}

path_entry_t* path_entry_new(vertex_id_t id)
{
    path_entry_t* entry = malloc(sizeof(path_entry_t));
    assert(entry);

    entry->id = id;
    return entry;
}

void path_entry_delete(path_entry_t* entry)
{
    free(entry);
}
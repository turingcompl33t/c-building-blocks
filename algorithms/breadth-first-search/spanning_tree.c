// spanning_tree.c
// A representation of the spanning tree constructed during BFS.

#include "spanning_tree.h"

#include <stdlib.h>

// ----------------------------------------------------------------------------
// Internal Declarations

typedef struct tree_item
{
    // The next item in the linked-list of tree item.
    struct tree_item* next;

    // The identifier for this vertex.
    vertex_id_t id;

    // The vertex identifier of this vertex's backpointer.
    vertex_id_t backpointer;

    // The length of the path from this vertex to the root.
    size_t path_length;
} tree_item_t;

struct spanning_tree
{
    // Head of the internal linked-list of tree items.
    tree_item_t* head;

    // The number of items in the spanning tree;
    // should always be equal to the number of 
    // vertices in input graph reachable from root
    size_t count;
};

static bool tree_contains(
    spanning_tree_t* tree, 
    vertex_id_t      id);

static tree_item_t* new_tree_item(
    vertex_id_t id, 
    vertex_id_t backpointer,
    size_t      path_length);
static void delete_tree_item(tree_item_t* item);

// ----------------------------------------------------------------------------
// Exported

spanning_tree_t* spanning_tree_new()
{
    spanning_tree_t* tree = malloc(sizeof(spanning_tree_t));
    if (NULL == tree)
    {
        return NULL;
    }

    tree->head  = NULL;
    tree->count = 0;

    return tree;
}

void spanning_tree_delete(spanning_tree_t* tree)
{
    if (NULL == tree)
    {
        return;
    }

    tree_item_t* current = tree->head;
    while (current != NULL)
    {
        tree_item_t* tmp = current->next;
        delete_tree_item(current);
        current = tmp;
    }

    free(tree);
}

bool spanning_tree_add(
    spanning_tree_t* tree, 
    vertex_id_t      id, 
    vertex_id_t      backpointer,
    size_t           path_length)
{
    if (NULL == tree || tree_contains(tree, id))
    {
        return false;
    }

    tree_item_t* item = new_tree_item(id, backpointer, path_length);
    if (NULL == item)
    {
        return false;
    }

    item->next = tree->head;
    tree->head = item;

    tree->count++;

    return true;
}

size_t spanning_tree_count(spanning_tree_t* tree)
{
    return (NULL == tree) ? 0 : tree->count;
}

void spanning_tree_for_each(
    spanning_tree_t* tree, 
    tree_iter_f      iter, 
    void*            ctx)
{
    if (NULL == tree)
    {
        return;
    }

    tree_item_t* current;
    for (current = tree->head; 
         current != NULL; 
         current = current->next)
    {
        iter(current->id, current->backpointer, current->path_length, ctx);
    }
}

// ----------------------------------------------------------------------------
// Internal

static bool tree_contains(
    spanning_tree_t* tree, 
    vertex_id_t      id)
{
    tree_item_t* current;
    for (current = tree->head; 
         current != NULL; 
         current = current->next)
    {
        if (current->id == id)
        {
            return true;
        }
    }

    return false;
}

static tree_item_t* new_tree_item(
    vertex_id_t id, 
    vertex_id_t backpointer,
    size_t      path_length)
{
    tree_item_t* item = malloc(sizeof(tree_item_t));
    if (NULL == item)
    {
        return NULL;
    }

    item->id          = id;
    item->backpointer = backpointer;
    item->path_length = path_length;

    return item;
}

static void delete_tree_item(tree_item_t* item)
{
    free(item);
}
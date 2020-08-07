// visited_set.c

#include "visited_set.h"

#include <assert.h>
#include <stdlib.h>

// ----------------------------------------------------------------------------
// Internal Declarations

// An individual entry in the visited set.
struct visited_entry
{
    // The next entry in the linked-list of entries.
    visited_entry_t* next;

    // The identifier for the vertex represented by this entry.
    vertex_id_t id;

    // The vertex prior to this vertex on minimum-weight path to root.
    vertex_id_t backpointer;

    // The cumulative weight of the path from root to this vertex.
    ssize_t weight;
};

// The visited set data structure.
struct visited_set
{
    visited_entry_t* head;
};

static visited_entry_t* new_visited_entry(
    vertex_id_t id, 
    vertex_id_t backpointer, 
    ssize_t     weight);
static void delete_visited_entry(visited_entry_t* entry);

// ----------------------------------------------------------------------------
// Exported

visited_set_t* set_new()
{
    visited_set_t* set = malloc(sizeof(visited_set_t));
    if (NULL == set)
    {
        return NULL;
    }

    set->head = NULL;

    return set;
}

void set_delete(visited_set_t* set)
{
    if (NULL == set)
    {
        return;
    }

    visited_entry_t* current = set->head;
    while (current != NULL)
    {
        visited_entry_t* tmp = current->next;
        delete_visited_entry(current);
        current = tmp;
    }

    free(set);
}

void set_add(
    visited_set_t* set, 
    vertex_id_t    id,
    vertex_id_t    backpointer,
    ssize_t        weight)
{
    if (NULL == set)
    {
        return;
    }

    assert(!set_contains(set, id));

    // TODO: less-lazy error handling
    visited_entry_t* entry = new_visited_entry(id, backpointer, weight);
    assert(entry);

    entry->next = set->head;
    set->head   = entry;
}

bool set_contains(
    visited_set_t* set, 
    vertex_id_t    id)
{
    if (NULL == set)
    {
        return false;
    }

    visited_entry_t* current;
    for (current = set->head;
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

ssize_t set_backpointer_for_id(
    visited_set_t* set, 
    vertex_id_t    id)
{
    if (NULL == set)
    {
        return NULL_VERTEX_ID;
    }

    visited_entry_t* current;
    for (current = set->head;
         current != NULL;
         current = current->next)
    {
        if (current->id == id)
        {
            return current->backpointer;
        }
    }

    // not found
    return NULL_VERTEX_ID;
}

ssize_t set_weight_for_id(
    visited_set_t* set, 
    vertex_id_t    id)
{
    if (NULL == set)
    {
        return NULL_EDGE_WEIGHT;
    }

    visited_entry_t* current;
    for (current = set->head;
         current != NULL;
         current = current->next)
    {
        if (current->id == id)
        {
            return current->weight;
        }
    }

    // not found
    return NULL_EDGE_WEIGHT;
}

// ----------------------------------------------------------------------------
// Internal

static visited_entry_t* new_visited_entry(
    vertex_id_t id, 
    vertex_id_t backpointer, 
    ssize_t     weight)
{
    visited_entry_t* entry = malloc(sizeof(visited_entry_t));
    if (NULL == entry)
    {
        return NULL;
    }

    entry->id          = id;
    entry->backpointer = backpointer;
    entry->weight      = weight;

    return entry;
}

static void delete_visited_entry(visited_entry_t* entry)
{
    free(entry);
}
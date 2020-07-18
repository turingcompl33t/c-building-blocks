// set.c
// Generic set data structure.

#include <stdlib.h>

#include "set.h"

// ----------------------------------------------------------------------------
// Internal Declarations

// An individual set item.
typedef struct set_item
{
    // Pointer to next item in the internal list.
    struct set_item* next;

    // Pointer to user-provided data.
    void* data;

    // Hash of the user-provided data.
    hash_t hash;
} set_item_t;

static set_item_t* new_item(void* data, hash_t hash);

// ----------------------------------------------------------------------------
// Exported

set_t* set_new(deleter_f deleter, hasher_f hasher)
{
    if (NULL == deleter || NULL == hasher)
    {
        return NULL;
    }

    set_t* s = malloc(sizeof(set_t));
    if (NULL == s)
    {
        return NULL;
    }

    s->count   = 0;
    s->head    = NULL;
    s->deleter = deleter;
    s->hasher  = hasher;

    return s;
}

void set_delete(set_t* set)
{
    if (NULL == set)
    {
        return;
    }

    set_item_t* current = set->head;
    while (current != NULL)
    {
        set_item_t* tmp = current->next;

        set->deleter(current->data);
        free(current);

        current = tmp;
    }

    free(set);
    set = NULL;
}

bool set_add(set_t* set, void* data)
{
    if (NULL == set)
    {
        return false;
    }

    // don't add a duplicate item
    if (set_contains(set, data))
    {
        return false;
    }

    // construct the new item
    set_item_t* item = new_item(data, set->hasher(data));
    if (NULL == item)
    {
        return false;
    }

    // splice the new item into the list
    item->next = set->head;
    set->head  = item;
    set->count++;

    return true;
}

bool set_remove(set_t* set, void* data)
{
    if (NULL == set || set->count == 0)
    {
        return false;
    }

    const hash_t query_hash = set->hasher(data);

    // walk the list and search for a matching item to remove
    set_item_t* current = set->head;
    set_item_t* prev    = NULL;
    while (current != NULL)
    {
        if (set->hasher(current->data) == query_hash)
        {
            // match
            break;
        }

        prev    = current;
        current = current->next;
    }

    if (NULL == current)
    {
        // exited loop without finding match
        return false;
    }

    // otherwise, we found our element, referred to by `current`

    if (NULL == prev)
    {
        // matched on first element
        set->head = current->next;
    }
    else
    {
        // general case
        prev->next = current->next;
    }
    
    set->count--;

    return true;
}

bool set_contains(set_t* set, void* data)
{
    if (NULL == set)
    {
        return false;
    }

    // compute the hash for the provided data 
    hash_t hash = set->hasher(data);

    // walk the list and determine if the hash is present
    set_item_t* current;
    bool found_match = false;

    for (current = set->head;
        current != NULL;
        current = current->next)
    {
        if (current->hash == hash)
        {
            found_match = true;
            break;
        }
    }

    return found_match;
}

size_t set_count(set_t* set)
{
    return (NULL == set) ? 0 : set->count;
}

void set_for_each(set_t* set, iterator_f iterator)
{
    if (NULL == set || set->count == 0)
    {
        return;
    }

    set_item_t* current;
    for (current = set->head;
        current != NULL;
        current = current->next)
    {
        iterator(current->data);
    }
}

// ----------------------------------------------------------------------------
// Internal

static set_item_t* new_item(void* data, hash_t hash)
{
    set_item_t* item = malloc(sizeof(set_item_t));
    if (NULL == item)
    {
        return NULL;
    }

    item->data = data;
    item->hash = hash;

    return item;
}
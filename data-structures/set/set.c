// set.c
// Generic set data structure.

#include <stdlib.h>

#include "set.h"

// An individual set item.
typedef struct set_item
{
    // Pointer to next item in the internal list.
    struct set_item* next;

    // Pointer to user-provided data.
    void* data;
} set_item_t;

set_t* set_new(deleter_f deleter, hasher_f hasher)
{
    return NULL;
}

void set_delete(set_t* set)
{
    return;
}

bool set_add(set_t* set, void* data)
{
    return false;
}

bool set_remove(set_t* set, void* data)
{
    return NULL;
}

bool set_contains(set_t* set, void* data)
{
    return false;
}

size_t set_count(set_t* set)
{
    return 0;
}

void set_for_each(set_t* set, iterator_f iterator)
{
    return;
}
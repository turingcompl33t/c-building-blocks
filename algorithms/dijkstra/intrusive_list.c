// intrusive_list.c
// Generic intrusive doubly-linked list data structure.

#include <stdlib.h>
#include <stdbool.h>

#include "intrusive_list.h"

// ----------------------------------------------------------------------------
// Internal Declarations

static bool list_is_empty(list_entry_t* head);

// ----------------------------------------------------------------------------
// Exported

void list_init(list_entry_t* head)
{
    // One might image using NULL values for flink and blink
    // to denote an empty list, but this implementation makes
    // the pointer swap operations cleaner in other functions.

    head->flink = head;
    head->blink = head;
}

void list_push_front(list_entry_t* head, list_entry_t* entry)
{
    if (NULL == head || NULL == entry)
    {
        return;
    }

    if (list_is_empty(head))
    {
        head->blink = entry;
    }

    entry->flink = head->flink;
    entry->blink = head;

    head->flink->blink = entry;
    head->flink = entry;
}

void list_push_back(list_entry_t* head, list_entry_t* entry)
{
    if (NULL == head || NULL == entry)
    {
        return;
    }

    if (list_is_empty(head))
    {
        head->flink = entry;
    }

    entry->blink = head->blink;
    entry->flink = head;

    head->blink->flink = entry;
    head->blink = entry;
}

list_entry_t* list_pop_front(list_entry_t* head)
{
    if (NULL == head || list_is_empty(head))
    {
        return NULL;
    }

    list_entry_t* popped = head->flink;

    popped->flink->blink = popped->blink;
    popped->blink->flink = popped->flink;

    return popped;
}

list_entry_t* list_pop_back(list_entry_t* head)
{
    if (NULL == head || list_is_empty(head))
    {
        return NULL;
    }

    list_entry_t* popped = head->blink;

    popped->flink->blink = popped->blink;
    popped->blink->flink = popped->flink;

    return popped;
}

void list_remove_entry(list_entry_t* head, list_entry_t* entry)
{
    if (NULL == head || entry == head)
    {
        return;
    }

    entry->flink->blink = entry->blink;
    entry->blink->flink = entry->flink;
}

size_t list_count(list_entry_t* head)
{
    if (NULL == head || list_is_empty(head))
    {
        return 0;
    }

    size_t count = 0;
    list_entry_t* current;
    for (current = head->flink; 
         current != head; 
         current = current->flink)
    {
        ++count;
    }

    return count;
}

list_entry_t* list_find(
    list_entry_t* head, 
    predicate_f   pred, 
    void*         ctx)
{
    if (NULL == head || list_is_empty(head))
    {
        return NULL;
    }

    bool found = false;

    list_entry_t* current;
    for (current = head->flink; 
         current != head; 
         current = current->flink)
    {
        if (pred(current, ctx))
        {
            found = true;
            break;
        }
    }

    return found ? current : NULL;
}

void list_for_each(
    list_entry_t* head, 
    list_iter_f   iter, 
    void*         ctx)
{
    if (NULL == head || list_is_empty(head))
    {
        return;
    }

    list_entry_t* current;
    for (current = head->flink; 
         current != head; 
         current = current->flink)
    {
        iter(current, ctx);
    }
}

// ----------------------------------------------------------------------------
// Internal

static bool list_is_empty(list_entry_t* head)
{
    return (head->flink == head) && (head->blink == head);
}
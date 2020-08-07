// intrusive_list.h
// Generic intrusive doubly-linked list data structure.

#ifndef INTRUSIVE_LIST_H
#define INTRUSIVE_LIST_H

#include <stddef.h>
#include <stdbool.h>

// An entry embedded in the intrusive linked-list.
typedef struct list_entry
{
    struct list_entry* flink;  // forward link
    struct list_entry* blink;  // backward link
} list_entry_t;

// The signature of the user-provided predicate.
typedef bool (*predicate_f)(list_entry_t*, void*);

// The signature of the user-provided iterator function.
typedef void (*list_iter_f)(list_entry_t*, void*);

// list_init()
// 
// Initializes a new intrusive list data structure.
//
// Arguments:
//  head - pointer to the list entry that represents head of the list
void list_init(list_entry_t* head);

// list_push_front()
//
// Pushes a list entry onto the front of the intrusive list.
//
// Arguments:
//  head  - pointer to the list entry that represents head of the list
//  entry - pointer to the entry to add to the list
void list_push_front(list_entry_t* head, list_entry_t* entry);

// list_push_back()
//
// Pushes a list entry onto the back of the intrusive list.
//
// Arguments:
//  head  - pointer to the list entry that represents head of the list
//  entry - pointer to the entry to add to the list
void list_push_back(list_entry_t* head, list_entry_t* entry);

// list_pop_front()
//
// Removes an entry from the front of the intrusive list.
//
// Arguments:
//  head - pointer to the list entry that represents head of the list
//
// Returns:
//  A pointer to the list entry removed from the intrusive list
//  NULL if the list is empty
list_entry_t* list_pop_front(list_entry_t* head);

// list_pop_back()
//
// Removes an entry from the back of the intrusive list.
//
// Arguments:
//  head - pointer to the list entry that represents head of the list
//
// Returns:
//  A pointer to the list entry removed from the intrusive list
//  NULL if the list is empty
list_entry_t* list_pop_back(list_entry_t* head);

// list_remove_entry()
//
// Removes the specified entry from the intrusive list.
//
// Arguments:
//  head  - pointer to the list entry that represents head of the list
//  entry - pointer to the list entry to be removed from the list 
void list_remove_entry(list_entry_t* head, list_entry_t* entry);

// list_count()
//
// Returns the count of elements in the list.
//
// Arguments:
//  head - pointer to the list entry that represents head of the list
//
// Returns:
//  The count of elements in the intrusive list
size_t list_count(list_entry_t* head);

// list_find()
//
// Find the item in the list that satisfies the given predicate.
//
// Arguments:
//  head - pointer to the list entry that represents head of the list
//  pred - the user-provided predicate function
//  ctx  - arbitrary user-provided context
//
// Returns:
//  A pointer to the list entry satisfying the given predicate on success
//  NULL on failure or not found
list_entry_t* list_find(
    list_entry_t* head, 
    predicate_f   pred, 
    void*         ctx);

// list_for_each()
//
// Invoke the provided callback on each item in the list.
//
// Arguments:
//  head - pointer to the list entry that represents head of the list
//  iter - the callback function invoked on each entry
//  ctx  - arbitrary user-provided context
void list_for_each(
    list_entry_t* head, 
    list_iter_f   iter, 
    void*         ctx);

#endif // INTRUSIVE_LIST_H
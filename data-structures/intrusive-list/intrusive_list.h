// intrusive_list.h
// Generic intrusive doubly-linked list data structure.

#ifndef INTRUSIVE_LIST_H
#define INTRUSIVE_LIST_H

#include <stddef.h>

// Background:
// 
// An "intrusive list" is a new way of thinking about
// linked-list data structures. Typically, we see linked-
// list implementations in which each element in the list
// contains the necessary pointers to maintain the list as
// well as the data being stored in each element. For 
// instance, in a linked-list designed to store `int` types,
// each node in the list might hold a pointer to the next 
// element in the list, as well as space for an integer value.
// For generic linked-lists, space for a generic pointer (void*)
// is typically provided in the list node type.
//
// In contrast, in an intrusive list, there is no list structure.
// Instead, only a list entry type is defined, along with operations
// that accept these list entries as parameters to manipulate the
// list. Then, users of the intrusive list library embed the list
// entry type within their own data structures, and when they want to
// add these arbitrary types to the list, the locate the address of
// the list entry within their structure and provide this to the
// list manipulation functions.

// An entry embedded in the intrusive linked-list.
typedef struct list_entry
{
    struct list_entry* flink;  // forward link
    struct list_entry* blink;  // backward link
} list_entry_t;

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

#endif // INTRUSIVE_LIST_H
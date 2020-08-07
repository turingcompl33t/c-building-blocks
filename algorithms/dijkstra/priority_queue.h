// priority_queue.h
// Simple linked-list based priority queue.

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdbool.h>

typedef struct priority_queue priority_queue_t;

// The type returned by the user-provided priority function.
typedef enum priority
{
    LESS,
    EQUAL,
    GREATER
} priority_t;

// The signature for the user-provided priority function.
typedef priority_t (*prioritizer_f)(void*, void*);

// The signature for the user-provided delete function.
typedef void (*deleter_f)(void*);

// queue_new()
//
// Construct a new priority queue.
//
// Arguments:
//  prioritizer - the function used to relative priority of items in the queue
//  deleter     - the function used to destroy items in the queue during delete
//
// Returns:
//  A pointer to a newly constructed priority queue on success
//  NULL on failure
priority_queue_t* queue_new(
    prioritizer_f prioritizer, 
    deleter_f     deleter);

// queue_delete()
//
// Destroy an existing priority queue.
//
// Arguments:
//  queue - pointer to priority queue instance
void queue_delete(priority_queue_t* queue);

// queue_push()
//
// Insert an item into the priority queue.
//
// Arguments:
//  queue - pointer to priority queue instance
//  value - the item to insert into the queue
//
// Returns:
//  `true` on successful addition of the item to the queue
//  `false` otherwise (allocation failure)
bool queue_push(priority_queue_t* queue, void* value);

// queue_pop()
//
// Remove the highest priority item from the queue.
//
// Arguments:
//  queue - pointer to priority queue instance
//
// Returns:
//  A pointer to the item removed from the queue on success
//  NULL on failure (invalid argument, empty queue)
void* queue_pop(priority_queue_t* queue);

#endif // PRIORITY_QUEUE_H
// queue.h
// Generic FIFO queue data structure.

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

typedef struct queue queue_t;

// The signature for the user-provided delete function.
typedef void (*deleter_f)(void*);

// queue_new()
//
// Construct a new queue
//
// Arguments:
//  deleter - the user-provided delete function
//
// Returns:
//  A pointer to a newly constructed queue object on success
//  NULL on failure
queue_t* queue_new(deleter_f deleter);

// queue_delete()
//
// Delete all of the items in the queue
// using the user-provided deleter, and
// subsequently destroy the queue itself.
//
// Arguments:
//  queue - pointer to the queue object
void queue_delete(queue_t* queue);

// queue_push()
//
// Insert a new item at the tail of the queue.
//
// Arguments:
//  queue - pointer to the queue object
//  value - the item to be added to the queue
//
// Returns:
//  `true` if the item is successfully added to the queue
//  `false` otherwise (invalid argument, allocation failure)
bool queue_push(queue_t* queue, void* value);

// queue_pop()
//
// Remove the item at the head of the queue.
//
// If the queue is empty, this function returns NULL.
//
// Arguments:
//  queue - pointer to the queue object
//
// Returns:
//  The item removed from the queue on success 
//  NULL on failure (invalid argument, empty queue)
void* queue_pop(queue_t* queue);

#endif // QUEUE_H
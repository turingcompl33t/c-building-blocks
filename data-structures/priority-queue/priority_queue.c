// priority_queue.c
// Simple linked-list based priority queue.

#include "priority_queue.h"

#include <stdlib.h>

priority_queue_t* queue_new(
    prioritizer_f prioritizer, 
    deleter_f     deleter)
{
    return NULL;
}   

void queue_delete(priority_queue_t* queue)
{
    return;
}

bool queue_push(priority_queue_t* queue, void* value)
{
    return false;
}

void* queue_pop(priority_queue_t* queue)
{
    return NULL;
}
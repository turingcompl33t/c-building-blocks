// queue.c
// Generic FIFO queue data structure.

#include "queue.h"

#include <stdlib.h>

queue_t* queue_new(deleter_f deleter)
{
    return NULL;
}

void queue_delete(queue_t* queue)
{
    return;
}

bool queue_push(queue_t* queue, void* value)
{
    return false;
}

void* queue_pop(queue_t* queue)
{
    return NULL;
}
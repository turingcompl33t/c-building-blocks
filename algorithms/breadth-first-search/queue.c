// queue.c
// Generic FIFO queue data structure.

#include "queue.h"

#include <stdlib.h>

// ----------------------------------------------------------------------------
// Internal Declarations

typedef struct queue_item
{
    struct queue_item* next;
    void*              value;
} queue_item_t;

struct queue
{
    queue_item_t* head;
    queue_item_t* tail;

    deleter_f deleter;
};

static bool queue_is_empty(queue_t* queue);

static queue_item_t* new_queue_item(void* value);
static void delete_queue_item(queue_item_t* item);

// ----------------------------------------------------------------------------
// Exported

queue_t* queue_new(deleter_f deleter)
{
    if (NULL == deleter)
    {
        return NULL;
    }

    queue_t* queue = malloc(sizeof(queue_t));
    if (NULL == queue)
    {
        return NULL;
    }

    queue->head = NULL;
    queue->tail = NULL;

    queue->deleter = deleter;

    return queue;
}

void queue_delete(queue_t* queue)
{
    if (NULL == queue)
    {
        return;
    }

    void* value;
    while ((value = queue_pop(queue)) != NULL)
    {
        queue->deleter(value);
    }

    free(queue);
}

bool queue_push(queue_t* queue, void* value)
{
    if (NULL == queue)
    {
        return false;
    }

    queue_item_t* item = new_queue_item(value);
    if (NULL == item)
    {
        return false;
    }

    if (queue_is_empty(queue))
    {
        queue->head = item;
        queue->tail = item;
    }
    else
    {
        // general case
        queue->tail->next = item;
        queue->tail       = item;
    }

    return true;
}

void* queue_pop(queue_t* queue)
{
    if (NULL == queue || queue_is_empty(queue))
    {
        return NULL;
    }

    queue_item_t* popped = queue->head;
    if (queue->tail == popped)
    {
        // only a single item in the queue
        queue->head = NULL;
        queue->tail = NULL;
    }
    else
    {
        // general case
        queue->head = popped->next;
    }

    void* tmp = popped->value;
    delete_queue_item(popped);

    return tmp;
}

// ----------------------------------------------------------------------------
// Internal

static bool queue_is_empty(queue_t* queue)
{
    return (NULL == queue->head) && (NULL == queue->tail);
}

static queue_item_t* new_queue_item(void* value)
{
    queue_item_t* item = malloc(sizeof(queue_item_t));
    if (NULL == item)
    {
        return NULL;
    }

    item->next  = NULL;
    item->value = value;

    return item;
}

static void delete_queue_item(queue_item_t* item)
{
    free(item);
}
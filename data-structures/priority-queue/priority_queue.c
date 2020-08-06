// priority_queue.c
// Simple linked-list based priority queue.

#include "priority_queue.h"

#include <stdlib.h>

// ----------------------------------------------------------------------------
// Exported

typedef struct queue_node
{
    // Linked-list entry.
    struct queue_node* next;

    // User-provided data.
    void* value;
} queue_node_t;

struct priority_queue
{
    queue_node_t* head;
    queue_node_t* tail;

    prioritizer_f prioritizer;
    deleter_f     deleter;
};

static bool queue_is_empty(priority_queue_t* queue);

static queue_node_t* new_node(void* value);
static void delete_node(queue_node_t* node);

// ----------------------------------------------------------------------------
// Exported

priority_queue_t* queue_new(
    prioritizer_f prioritizer, 
    deleter_f     deleter)
{
    priority_queue_t* queue = malloc(sizeof(priority_queue_t));
    if (NULL == queue)
    {
        return NULL;
    }

    queue->head = NULL;
    queue->tail = NULL;

    queue->prioritizer = prioritizer;
    queue->deleter     = deleter;

    return queue;
}   

void queue_delete(priority_queue_t* queue)
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

bool queue_push(priority_queue_t* queue, void* value)
{
    if (NULL == queue)
    {
        return false;
    }

    queue_node_t* node = new_node(value);
    if (NULL == node)
    {
        return false;
    }

    if (queue_is_empty(queue))
    {
        queue->head = node;
        queue->tail = node;
        return true;
    }

    bool inserted = false;

    queue_node_t* prev = NULL;
    queue_node_t* curr = queue->head;
    while (curr != NULL)
    {
        priority_t r = queue->prioritizer(node->value, curr->value);
        if (GREATER == r || EQUAL == r)
        {
            if (NULL == prev)
            {
                // insert at the head of the list
                node->next  = queue->head;
                queue->head = node;
            }
            else
            {
                // general case
                prev->next = node;
                node->next = curr;
            }

            inserted = true;
            break;
        }

        prev = curr;
        curr = curr->next;
    }

    if (!inserted)
    {
        // new node is lowest-priority item
        node->next        = NULL;
        queue->tail->next = node;
        queue->tail       = node;
    }

    return true;
}

void* queue_pop(priority_queue_t* queue)
{
    if (NULL == queue || queue_is_empty(queue))
    {
        return false;
    }

    queue_node_t* popped = queue->head;

    if (queue->tail == popped)
    {
        // a single item in the queue; need to update tail as well
        queue->head = NULL;
        queue->tail = NULL;
    }
    else
    {
        // general case
        queue->head = popped->next;
    }

    void* tmp = popped->value;
    delete_node(popped);

    return tmp;
}

// ----------------------------------------------------------------------------
// Internal

static bool queue_is_empty(priority_queue_t* queue)
{
    return (NULL == queue->head) && (NULL == queue->tail);
}

static queue_node_t* new_node(void* value)
{
    queue_node_t* node = malloc(sizeof(queue_node_t));
    if (NULL == node)
    {
        return NULL;
    }

    node->next  = NULL;
    node->value = value;

    return node;
}

static void delete_node(queue_node_t* node)
{
    free(node);
}
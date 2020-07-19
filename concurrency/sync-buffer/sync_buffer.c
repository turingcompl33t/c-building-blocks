// sync_buffer.c
// A general internally-synchronized bounded-buffer data structure.

#include <stdlib.h>

#include "sync_buffer.h"

// ----------------------------------------------------------------------------
// Internal Declarations

// A single element in the internal linked list.
typedef struct buffer_item
{
    // Pointer to the next item in the buffer.
    struct buffer_item* next;

    // User-provided data.
    void* data;
} buffer_item_t;

static void put_unsafe(sync_buffer_t* buffer, buffer_item_t* item);
static buffer_item_t* get_unsafe(sync_buffer_t* buffer);

static bool buffer_empty_unsafe(sync_buffer_t* buffer);
static bool buffer_full_unsafe(sync_buffer_t* buffer);

static buffer_item_t* new_item(void* data);
static void destroy_item(buffer_item_t* item);

// ----------------------------------------------------------------------------
// Exported

sync_buffer_t* sync_buffer_new(const size_t capacity)
{
    if (0 == capacity)
    {
        return NULL;
    }

    sync_buffer_t* buffer = malloc(sizeof(sync_buffer_t));
    if (NULL == buffer)
    {
        return NULL;
    }

    // default initialization for mutex
    const int lock_init = pthread_mutex_init(&buffer->lock, NULL);

    // default initialization for condition variables
    const int nonempty_init = pthread_cond_init(&buffer->nonempty, NULL);
    const int nonfull_init  = pthread_cond_init(&buffer->nonfull, NULL);\

    if (lock_init != 0 || nonempty_init != 0 || nonfull_init != 0)
    {
        // initialization of one or more synchronization primitives failed
        free(buffer);
        return NULL;
    }

    buffer->head = NULL;
    buffer->tail = NULL;

    buffer->count    = 0;
    buffer->capacity = capacity;

    return buffer;
}

bool sync_buffer_delete(sync_buffer_t* buffer)
{
    if (NULL == buffer)
    {
        return false;
    }

    if (!buffer_empty_unsafe(buffer))
    {
        // buffer is not empty, bail
        return false;
    }

    pthread_mutex_destroy(&buffer->lock);
    pthread_cond_destroy(&buffer->nonempty);
    pthread_cond_destroy(&buffer->nonfull);

    free(buffer);

    return true;
}

bool sync_buffer_put(sync_buffer_t* buffer, void* data)
{
    if (NULL == buffer)
    {
        return false;
    }

    buffer_item_t* item = new_item(data);
    if (NULL == item)
    {
        return false;
    }

    // acquire exclusive access to the buffer
    pthread_mutex_lock(&buffer->lock);

    // block until the buffer is nonfull
    while (buffer_full_unsafe(buffer))
    {
        pthread_cond_wait(&buffer->nonfull, &buffer->lock);
    }

    // buffer is now nonfull and we have exclusive access;
    put_unsafe(buffer, item);

    // release exclusive access to the buffer
    pthread_mutex_unlock(&buffer->lock);

    // notify consumers that item has been added
    pthread_cond_signal(&buffer->nonempty);

    return true;
}

bool sync_buffer_try_put(sync_buffer_t* buffer, void* data)
{
    if (NULL == buffer)
    {
        return false;
    }

    buffer_item_t* item = new_item(data);
    if (NULL == item)
    {
        return false;
    }

    // acquire exclusive access to the buffer
    pthread_mutex_lock(&buffer->lock);

    // determine if buffer is full
    if (buffer_full_unsafe(buffer))
    {
        // buffer is full, bail on put operation
        pthread_mutex_unlock(&buffer->lock);
        free(item);
        return false;
    }

    // buffer is nonfull and we have exclusive access
    put_unsafe(buffer, item);

    // release exclusive access to the buffer
    pthread_mutex_unlock(&buffer->lock);

    // notify consumers that item has been added
    pthread_cond_signal(&buffer->nonempty);

    return true;
}

void* sync_buffer_get(sync_buffer_t* buffer)
{
    if (NULL == buffer)
    {
        return NULL;
    }

    // acquire exclusive access to the buffer
    pthread_mutex_lock(&buffer->lock);

    // block until buffer is nonempty
    while (buffer_empty_unsafe(buffer))
    {
        pthread_cond_wait(&buffer->nonempty, &buffer->lock);
    }

    // buffer is now nonempty and we have exclusive access;
    buffer_item_t* item = get_unsafe(buffer);

    // release exclusive access to the buffer
    pthread_mutex_unlock(&buffer->lock);

    // notify producers that item was removed
    pthread_cond_signal(&buffer->nonfull);

    void* data = item->data;
    destroy_item(item);

    return data;
}

void* sync_buffer_try_get(sync_buffer_t* buffer)
{
    if (NULL == buffer)
    {
        return NULL;
    }

    // acquire exclusive access to the buffer
    pthread_mutex_lock(&buffer->lock);

    // determine if buffer is nonempty
    if (buffer_empty_unsafe(buffer))
    {
        // buffer is empty, bail on get operation
        pthread_mutex_unlock(&buffer->lock);
        return NULL;
    }

    // buffer is nonempty and we have exclusive access;
    buffer_item_t* item = get_unsafe(buffer);

    // release exclusive access to the buffer
    pthread_mutex_unlock(&buffer->lock);

    // notify producers that item was removed
    pthread_cond_signal(&buffer->nonfull);

    void* data = item->data;
    destroy_item(item);

    return data;
}

// ----------------------------------------------------------------------------
// Internal

static void put_unsafe(sync_buffer_t* buffer, buffer_item_t* item)
{
    // insert at the tail of the buffer
    
    if (buffer_empty_unsafe(buffer))
    {
        // only need to touch head if buffer is empty
        buffer->head = item;
    }
    else
    {
        // in general case of nonempty buffer, update the
        // next point for the current tail
        buffer->tail->next = item;
    }

    item->next   = NULL;
    buffer->tail = item;

    buffer->count++;
}

static buffer_item_t* get_unsafe(sync_buffer_t* buffer)
{
    // remove from the head of the buffer 

    buffer_item_t* item = buffer->head;

    if (1 == buffer->count)
    {
        // only need to touch tail pointer when buffer will become empty
        buffer->tail = NULL;
    }

    buffer->head = item->next;

    buffer->count--;

    return item;
}

static bool buffer_empty_unsafe(sync_buffer_t* buffer)
{
    return 0 == buffer->count; 
}

static bool buffer_full_unsafe(sync_buffer_t* buffer)
{
    return buffer->count == buffer->capacity;
}

static buffer_item_t* new_item(void* data)
{
    buffer_item_t* item = malloc(sizeof(buffer_item_t));
    if (NULL == item)
    {
        return NULL;
    }

    item->data = data;
    return item;
}

static void destroy_item(buffer_item_t* item)
{
    free(item);
}
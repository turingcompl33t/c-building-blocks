// ring_buffer.c
// A single-threaded circular buffer.

#include "ring_buffer.h"

#include <string.h>
#include <stdlib.h>

// ----------------------------------------------------------------------------
// Internal Declarations

struct ring_buffer
{
    // The internal representation of the buffer.
    void** inner;

    // Head and tail indicies.
    size_t head;
    size_t tail;

    // The mask to compute indicies into the internal
    // buffer from head and tail indicies.
    size_t mask;

    // The user-provided delete function.
    deleter_f deleter;
};

static size_t mask(ring_buffer_t* buffer, size_t index);

static bool is_empty(ring_buffer_t* buffer);
static bool is_full(ring_buffer_t* buffer);

static size_t next_power_of_two(size_t v);
static size_t next_power_of_two_32(size_t v);
static size_t next_power_of_two_64(size_t v);

// ----------------------------------------------------------------------------
// Exported

ring_buffer_t* buffer_new(
    size_t    capacity, 
    deleter_f deleter)
{
    if (0 == capacity || NULL == deleter)
    {
        return NULL;
    }

    ring_buffer_t* buffer = malloc(sizeof(ring_buffer_t));
    if (NULL == buffer)
    {
        return NULL;
    }

    const size_t size = next_power_of_two(capacity);

    void** inner = calloc(size, sizeof(void*));
    if (NULL == inner)
    {
        free(inner);
        return NULL;
    }

    buffer->inner   = inner;
    buffer->head    = 0;
    buffer->tail    = 0;
    buffer->mask    = (size - 1);
    buffer->deleter = deleter;

    return buffer;
}

void buffer_delete(ring_buffer_t* buffer)
{
    if (NULL == buffer)
    {
        return;
    }

    for (size_t i = 0; i < (buffer->mask + 1); ++i)
    {
        if (buffer->inner[i] != NULL)
        {
            buffer->deleter(buffer->inner[i]);
        }
    }

    free(buffer);
}

bool buffer_put(ring_buffer_t* buffer, void* value)
{
    if (NULL == buffer || is_full(buffer))
    {
        return false;
    }

    buffer->inner[mask(buffer, buffer->head++)] = value;

    return true;
}

bool buffer_get(ring_buffer_t* buffer, void** value)
{
    if (NULL == buffer || NULL == value)
    {
        return false;
    }

    if (is_empty(buffer))
    {
        *value = NULL;
    }
    else
    {
        *value = buffer->inner[mask(buffer, buffer->tail++)];
    }

    return true;
}

void buffer_clear(ring_buffer_t* buffer)
{
    if (NULL == buffer)
    {
        return;
    }

    buffer->head = 0;
    buffer->tail = 0;

    const size_t capacity = (buffer->mask + 1);
    memset(buffer->inner, '\0', sizeof(void*)*capacity); 
}

// ----------------------------------------------------------------------------
// Internal

// Compute the masked index for the buffer.
static size_t mask(ring_buffer_t* buffer, size_t index)
{
    return (buffer->mask & index);
}

// Determine if the buffer is empty.
static bool is_empty(ring_buffer_t* buffer)
{
    return (buffer->head == buffer->tail);
}

// Determine if the buffer is full.
static bool is_full(ring_buffer_t* buffer)
{
    return (mask(buffer, buffer->head) 
        == mask(buffer, buffer->tail)) 
        && !(buffer->head == buffer->tail);
}

// Compute the next greater power of 2.
static size_t next_power_of_two(size_t v)
{
    return (sizeof(size_t) == 8) 
        ? next_power_of_two_64(v) 
        : next_power_of_two_32(v);
}

// https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
static size_t next_power_of_two_32(size_t v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    return ++v;
}

// https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
static size_t next_power_of_two_64(size_t v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v |= v >> 32;
    return ++v;
}
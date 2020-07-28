// ring_buffer.c
// A single-threaded circular buffer.

#include "ring_buffer.h"

#include <stdlib.h>

ring_buffer_t* buffer_new(
    size_t    capacity, 
    deleter_f deleter)
{
    return NULL;
}

void buffer_delete(ring_buffer_t* buffer)
{
    return;
}

bool buffer_put(ring_buffer_t* buffer, void* value)
{
    return false;
}

bool buffer_get(ring_buffer_t* buffer, void** value)
{
    return false;
}

void buffer_clear(ring_buffer_t* buffer)
{
    return;
}
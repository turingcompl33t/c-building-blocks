// sync_buffer.c
// A general internally-synchronized bounded-buffer data structure.

#include "sync_buffer.h"

sync_buffer_t* sync_buffer_new(const size_t capacity)
{
    return NULL;   
}

bool sync_buffer_delete(sync_buffer_t* buffer)
{
    return false;
}

bool sync_buffer_put(sync_buffer_t* buffer, void* data)
{
    return false;
}

bool sync_buffer_try_put(sync_buffer_t* buffer, void* data)
{
    return false;
}

void* sync_buffer_get(sync_buffer_t* buffer)
{
    return NULL;
}

void* sync_buffer_try_get(sync_buffer_t* buffer)
{
    return NULL;
}
// ring_buffer.h
// A single-threaded circular buffer.

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stddef.h>
#include <stdbool.h>

typedef struct ring_buffer ring_buffer_t;

// The signature for the user-provided delete function.
typedef void (*deleter_f)(void*);

// buffer_new()
//
// Construct a new ring buffer.
//
// Arguments:
//  capacity - the minimum capacity of the ring buffer;
//             the constructed buffer may have a larger capacity
//  deleter  - the user-provided delete function
//
// Returns:
//  A pointer to a newly constructed ring buffer on success
//  NULL on failure
ring_buffer_t* buffer_new(
    size_t    capacity, 
    deleter_f deleter);

// buffer_delete()
//
// Destroy a ring buffer instance.
//
// Arguments:
//  buffer - pointer to an existing ring buffer instance
void buffer_delete(ring_buffer_t* buffer);

// buffer_put()
//
// Put `value` into the ring buffer.
//
// If the buffer is currently full, `value` is
// not inserted and `false` is returned. 
//
// Arguments:
//  buffer - pointer to an existing ring buffer instance
//  value  - the value to insert into the buffer
//
// Returns:
//  `true` if `value` was inserted into the buffer
//  `false` otherwise
bool buffer_put(ring_buffer_t* buffer, void* value);

// buffer_get()
//
// Get a value out of the buffer.
//
// If the buffer is currently empty, *value is set
// to NULL and `false` is returned.
//
// Arguments:
//  buffer - pointer to an existing ring buffer instance
//  value  - pointer to the locationt that receives retrieved value
//
// Returns:
//  `true` if a value was removed from the buffer
//  `false` otherwise
bool buffer_get(ring_buffer_t* buffer, void** value);

// buffer_clear()
//
// Reset the buffer to an empty state.
//
// Arguments:
//  buffer - pointer to an existing ring buffer instance
void buffer_clear(ring_buffer_t* buffer);

#endif // RING_BUFFER_H
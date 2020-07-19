// sync_buffer.h
// A general internally-synchronized bounded-buffer data structure.

#ifndef SYNC_BUFFER_H
#define SYNC_BUFFER_H

#include <stddef.h>
#include <stdbool.h>
#include <pthread.h>

struct buffer_item;

// The synchronized buffer data structure.
typedef struct sync_buffer {

    // Head and tail pointers for internal linked-list.
    struct buffer_item* head;
    struct buffer_item* tail;

    // The lock that guards exclusive access to the buffer.
    pthread_mutex_t lock;

    // Signaled when an item is added to the buffer.
    pthread_cond_t nonempty;

    // Signaled when an item is removed from the buffer.
    pthread_cond_t nonfull;

    // The current count of items in the buffer.
    size_t count;

    // The maximum capacity of the buffer.
    size_t capacity;
} sync_buffer_t;

// sync_buffer_new()
//
// Construct a new synchronized buffer data structure.
//
// This function IS NOT threadsafe.
//
// Arguments:
//  capacity - the maximum capacity of the buffer
//
// Returns:
//  A pointer to a newly constructed buffer on success
//  NULL on failure
sync_buffer_t* sync_buffer_new(const size_t capacity);

// sync_buffer_delete()
//
// Destroy an existing synchronized buffer data structure.
//
// This function IS NOT threadsafe. Internally, it does
// not acquire exclusive access to the buffer prior to 
// accessing its members. Therefore, it is undefined
// behavior to call this function while any other operations
// on the buffer are underway in other thread contexts.
//
// Note that this function DOES NOT deallocate storage
// utilized by the items stored in the buffer. If the
// buffer is nonempty when this function is called,
// it fails to destroy the buffer and returns `false`.
// Users of this data structure are therefore responsible
// for emptying the buffer prior to calling this function.
//
// Arguments:
//  buffer - pointer to an existing buffer data structure
//
// Returns:
//  `true` if the buffer is successfully destroyed
//  `false` otherwise
bool sync_buffer_delete(sync_buffer_t* buffer);

// sync_buffer_put()
//
// Insert a new item into the synchronized buffer.
//
// This function is threadsafe. It blocks until it
// is able to acquire exclusive access to the buffer,
// and attempts to insert a new item into it. In the
// event that the buffer is full, this function blocks
// until the number of items in the buffer is lower 
// than the buffer's capacity and it is successfully
// able to insert the new data.
// 
// Arguments:
//  buffer - pointer to an existing buffer data structure
//  data   - user-provided data to insert
//
// Returns:
//  `true` if the new item is successfully inserted
//  `false` on failure (allocation failure)
bool sync_buffer_put(sync_buffer_t* buffer, void* data);

// sync_buffer_try_put()
//
// Insert a new item into the synchronized buffer.
//
// This function is threadsafe. It blocks until it
// is able to acquire exclusive access to the buffer,
// and attempts to insert a new item into it. In the
// event that the buffer is full, this function returns
// immediately without waiting for the number of items
// in the buffer to allow for successful insertion.
// 
// Arguments:
//  buffer - pointer to an existing buffer data structure
//  data   - user-provided data to insert
//
// Returns:
//  `true` if the data was successfully inserted
//  `false` otherwise
bool sync_buffer_try_put(sync_buffer_t* buffer, void* data);

// sync_buffer_put()
//
// Retrieve data from the synchronized buffer.
//
// This function is threadsafe. It blocks until it
// is able to acquire exclusive access to the buffer,
// and attempts to remove an existing item from it.
// In the event that the buffer is full, this function
// blocks until the buffer is nonempty and it is 
// successfully able to remove an item from the buffer.
//
// Arguments:
//  buffer - pointer to an existing buffer data structure
//
// Returns:
//  A pointer to the user data retrieved from the buffer
void* sync_buffer_get(sync_buffer_t* buffer);

// sync_buffer_try_put()
//
// Retrieve data from the synchronized buffer.
//
// This function is threadsafe. It blocks until it
// is able to acquire exclusive access to the buffer,
// and attempts to remove an existing item from it.
// In the event that the buffer is full, this function
// returns immediately without retrieving any data.
//
// Arguments:
//  buffer - pointer to an existing buffer data structure
//
// Returns:
//  A pointer to the user data retrieved from the buffer
//  NULL if no data is retrieved from buffer 
void* sync_buffer_try_get(sync_buffer_t* buffer);

#endif // SYNC_BUFFER_H
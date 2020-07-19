// sync_buffer.h
// A general internally-synchronized bounded-buffer data structure.

#ifndef SYNC_BUFFER_H
#define SYNC_BUFFER_H

#include <stddef.h>
#include <stdbool.h>
#include <pthread.h>

// Background:
//
// The "bounded buffer" problem or "producer-consumer"
// problem is a fundamental problem in concurrent programming.
// This module asks you to implement an internally-synchronized 
// data structure that solves the bounded buffer problem.
//
// A bounded buffer is simply a data structure that is capable 
// of storing up to a fixed number of items at any one time.
// That is, at any point after the construction of the buffer,
// the total number of items in the buffer is in the range:
//
//                  [0, CAPACITY]
//
// This may not seem like a terribly difficuly invariant to
// maintain. However, complications arise when multiple threads
// of execution are introduced. In this scenario, the buffer
// must implement some form of internal synchronization to 
// protect its invariants. Internal synchronization simply means
// that the synchronization provided by the buffer data structure
// is not visible to consumers of the buffer - any synchronization
// primitives required to provide thread-safety are maintained
// internally, and users of the buffer may simply call put()
// and get() and never worry about providing synchronization 
// themselves.
//
// As alluded to above, the fundamental API of a synchronized
// buffer consists of put() and get() operations. The put() operation
// inserts a new item into the buffer while the get() operation
// removes an item.
//
// In a single-threaded environment, the semantics for these two
// operations are straightforward: if put() is called on a full
// buffer, the operation fails. Likewise, if get() is called on an
// empty buffer, the operation fails. These semantics are a direct
// implication of the fact that in a single-threaded environment,
// the state of the buffer cannot be altered in any way outside of
// the main (sole) thread of execution. Thus, a failed put() operation
// must be followed by a get() operation prior to retrying with 
// an expectation of success, and likewise a failed get()
// operation must be followed by a put() operation prior to retrying.
//
// These semantics are altered, however, in a multithreaded environment.
// Now, a failed put() operation, indicating the buffer is currently full,
// does not require a return and subsequent retry in order to succeed.
// This results from the fact that while one thread is in the midst of
// a put() operation that is currently stalled because the buffer is full,
// another thread may enter the buffer's critical region and perform a
// get() operation that subsequently allows the producer thread performing
// the put() to succeed. The situation is analogous for a failed get()
// and a concurrent put().
//
// For this reason, a synchronized bounded buffer intended for multithreaded
// environments must support two variants of the put() and get() operations:
//
// - put() and try_put(): put() blocks the calling thread in the event that 
//   the buffer is full and waits to be notified by another thread that
//   the buffer is now nonfull such that the operation can proceed. try_put(),
//   in contrast, does not block the calling thread in the event that the 
//   buffer is full when the operation enters the critical region.
//
// - get() and try_get(): get() blocks the calling thread in the event that
//   the buffer is empty and waits to be notified by another thread that
//   the buffer now nonempty such that the operation can proceed. try_get(),
//   in contrast, does not block the calling thread in the event that the
//   buffer is empty when the operation enters the critical region.
//
// Supporting both APIs allows users of the synchronized buffer to store
// items into and load items from the buffer in a way that suits
// the particular use case in question. 
//
// NOTE: Although the API for the synchronized buffer is 
// agnostic of the order in which items are inserted and removed,
// such buffers are typically implemented with first-in first-out
// semantics and thus behave functionally identically to a 
// synchronized queue.
//
// NOTE: The declaration of the sync_buffer type below
// presumes the use of the pthread API for multithreaded
// programming. This is not strictly necessary, and 
// indeed the ISO C11 thread support library may be the
// more portable choice. However, the documentation and
// online support for the pthread API far exceeds that for
// the C11 thread support library in both volume and quality,
// so I chose to use pthreads for the purposes of this module.

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
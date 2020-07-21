// stack.h
// Generic stack (last in, first out) data structure.

#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include <stdbool.h>

struct stack_item;

// The stack data structure.
typedef struct stack
{
    // The number of elements in the stack. 
    size_t count;

    // Pointer to the current top of the stack.
    struct stack_item* top;
} lstack_t;

// stack_new()
//
// Construct a new stack data structure.
//
// Returns:
//  A pointer to the newly constructed stack on success.
//  Null on failure.
lstack_t* stack_new(void);

// stack_delete()
//
// Destroy an existing stack data structure.
//
// Note that while a stack stores arbitrary data for users,
// it does not take complete ownership of that data because
// the stack itself knows nothing about how to destroy the
// data that it maintains. For this reason, the stack needs
// some other mechanism by which it can prevent memory leaks
// caused by (common) improper usage. In this implementation,
// the stack_delete() method returns a boolean denoting whether
// the destruction operation succeeded or failed. In the event
// that the stack is not empty (count > 0), desutruction of
// the stack fails, and the function returns `false`. Thus, 
// it is up to users of the stack to check the return value
// of stack_delete() and if it returns `false`, call stack_pop()
// in a loop and manually destroy all of the stack's items prior 
// to calling stack_delete() again to complete the operation.
//
// Returns:
//  `true` on successful destruction of the stack
//  `false` in the event the stack is nonempty
bool stack_delete(lstack_t* stack);

// stack_push()
// 
// Push a new item onto the top of the stack.
//
// This function returns `false` in the event
// that the specified item is failed to be
// pushed onto the stack, for any reason.
//
// Returns:
//  `true` on successful push
//  `false` otherwise
bool stack_push(lstack_t* stack, void* data);

// stack_pop()
//
// Pop an item off the top of the stack.
//
// If an invalid stack pointer (`stack`) is passed
// as an argument to this function, or the stack
// is currently empty (count == 0), then this
// operation returns NULL.
//
// Returns:
//  A pointer to the user data popped from the stack on success.
//  NULL on failure (invalid stack, empty stack)
void* stack_pop(lstack_t* stack);

// stack_peek()
//
// Return the user data stored in the topmost stack
// item without removing the item from the stack.
//
// As with stack_pop(), this fuction returns NULL in
// the event that an invalid stack pointer (`stack`)
// is passed to the function, or if the stack is empty.
//
// Returns:
//  A pointer to the peeked data on success
//  NULL on failure (invalid stack, empty stack)
void* stack_peek(lstack_t* stack);

// stack_count()
//
// Return the current count of items on the stack.
//
// In the event that an invlaid stack pointer (`stack`)
// is passed to this function, it returns 0.
//
// Returns:
//  The current count of items on the stack.
size_t stack_count(lstack_t* stack);

#endif // STACK_H

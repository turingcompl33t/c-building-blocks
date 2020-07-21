// stack.c
// Generic stack (last in, first out) data structure.

#include "stack.h"

// An individual item on the stack.
typedef struct stack_item
{
    // A pointer to the item below this on the stack.
    struct stack_item* below;
    
    // A pointer to arbitrary user-provided data.
    void* data;
} stack_item_t;

lstack_t* stack_new(void)
{
    return NULL;
}

bool stack_delete(lstack_t* stack)
{
    return false;
}

bool stack_push(lstack_t* stack, void* data)
{
    return false;
}

void* stack_pop(lstack_t* stack)
{
    return NULL;
}

void* stack_peek(lstack_t* stack)
{
    return NULL;
}

size_t stack_count(lstack_t* stack)
{
    return 0;
}

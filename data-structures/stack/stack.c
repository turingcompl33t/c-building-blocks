// stack.c
// Generic stack (last in, first out) data structure.

#include <stdlib.h>

#include "stack.h"

// ----------------------------------------------------------------------------
// Internal Types

// An individual item on the stack.
typedef struct stack_item
{
    // A pointer to the item below this on the stack.
    struct stack_item* below;
    
    // A pointer to arbitrary user-provided data.
    void* data;
} stack_item_t;

// ----------------------------------------------------------------------------
// Internal Prototypes

static stack_item_t* new_item(void* data);

// ----------------------------------------------------------------------------
// Exported

stack_t* stack_new(void)
{
    stack_t* stack = malloc(sizeof(stack_t));
    if (NULL == stack)
    {
        return NULL;
    }

    stack->top   = NULL;
    stack->count = 0;

    return stack;
}

bool stack_delete(stack_t* stack)
{
    if (NULL == stack || stack->count > 0)
    {
        return false;
    }

    // the stack is empty, safe to delete
    free(stack);
    stack = NULL;

    return true;
}

bool stack_push(stack_t* stack, void* data)
{
    if (NULL == stack)
    {
        return false;
    }

    stack_item_t* item = new_item(data);
    if (NULL == item)
    {
        return false;
    }

    item->below = stack->top;

    stack->top = item;
    stack->count++;

    return true;
}

void* stack_pop(stack_t* stack)
{
    if (NULL == stack || stack->count == 0)
    {
        return NULL;
    }

    stack_item_t* popped = stack->top;

    stack->top = popped->below;
    stack->count--;

    void* data = popped->data;

    free(popped);

    return data;
}

void* stack_peek(stack_t* stack)
{
    if (NULL == stack || stack->count == 0)
    {
        return NULL;
    }

    return stack->top->data;
}

size_t stack_count(stack_t* stack)
{
    return (NULL == stack) ? 0 : stack->count; 
}

// ----------------------------------------------------------------------------
// Internal

static stack_item_t* new_item(void* data)
{
    stack_item_t* item = malloc(sizeof(stack_item_t));
    if (NULL == item)
    {
        return NULL;
    }

    item->data = data;
    return item;
}
// stack.c
// Generic stack (last in, first out) data structure.

#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

// An individual item on the stack.
typedef struct stack_item
{
    // A pointer to the item below this on the stack.
    struct stack_item* below;

    // A pointer to arbitrary user-provided data.
    void* data;
} stack_item_t;

//static stack_item_t* new_item(void* data);

stack_t* stack_new(void)
{
    stack_t * stack = malloc(sizeof(stack_t));
    if(stack==NULL){
      return NULL;
    }
    stack->count = 0;
    stack->top = NULL;
    return stack;
}

bool stack_delete(stack_t* stack)
{
    if (stack==NULL||stack->count >0){
      return false;
    }
    free(stack);
    stack = NULL; //makes the address that the stack
    //points to NULL. removes a dangling pointer and
    //ensures that the data that's still held at the
   //address doesn't cause you errors later if you call
   //stack again.
    return true;
}

bool stack_push(stack_t* stack, void* data)
{
    if(stack==NULL){
      return false;
    }
    stack_item_t* news = malloc(sizeof(stack_item_t));
    if(news==NULL){
      return false;
    }
    news->data = data;
    //if want to use the static function new_item
    //stack_item_t* news = new_item(data);
    //if(NULL==item){
        //return false;
   //}

    news->below = stack->top;
    stack->top = news;
    stack->count++;
    return true;
}

void* stack_pop(stack_t* stack)
{
    if(stack==NULL||stack->count==0){
      return NULL;
    }
    stack_item_t* temp = stack->top;

    void* datap = temp->data;
    stack->top = temp->below;
    stack->count--;
    free(temp);
    return datap;
}

void* stack_peek(stack_t* stack)
{
    if(stack==NULL||stack->count==0){
      return NULL;
    }
    stack_item_t* temp = stack->top;
    if(temp->data!=NULL){
      void * datap = temp->data;
      return datap;
    }
    return NULL;
}

size_t stack_count(stack_t* stack)
{
    if(stack==NULL){
      return 0;
    }
    return stack->count;
}

//------------------------------------//
//code for new_item
//static stack_item_t* new_item(void* data){
  //stack_item_t* item = malloc(sizeof(stack_item_t));
  //if(NULL == item){
    //return NULL;
  //}
  //item->data = data;
  //return item;
//}

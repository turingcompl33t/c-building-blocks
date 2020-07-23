// set.c
// Generic set data structure.

#include <stdlib.h>
#include <stdio.h>

#include "set.h"

// An individual set item.
typedef struct set_item
{
    // Pointer to next item in the internal list.
    struct set_item* next;

    // Pointer to user-provided data.
    void* data;
} set_item_t;

set_t* set_new(deleter_f deleter, hasher_f hasher)
{
    if (NULL==deleter||NULL==hasher){
      return NULL;
    }
    set_t * set = malloc(sizeof(set_t));
    if (set == NULL){
      return NULL;
    }

    set->head = NULL;
    set->deleter = deleter;
    set->hasher = hasher;
    set->count = 0;
    return set;
}

void set_delete(set_t* set)
{
    if(NULL==set){
      return;
    }
    set_item_t* curr = set->head;
    while(curr!=NULL){
      set_item_t * temp = curr->next;
      set->deleter(curr->data);
      free(curr);
      set->count--;
      curr = temp;
    }
    free(set);
    set = NULL;
    return;
}

bool set_add(set_t* set, void* data)
{
    if(set==NULL){
      return false;
    }
    if(set_contains(set,data)){
      return false;
    }
    set_item_t * newsi = malloc(sizeof(set_item_t));
    if(newsi == NULL){
      return false;
    }
    newsi->data = data;
    newsi->next = set->head;
    set->head = newsi;
    set->count++;
    return true;
}

bool set_remove(set_t* set, void* data)
{
    if(set==NULL||set->count==0){
      return false;
    }
    set_item_t * temp = set->head;
    set_item_t * prev = NULL;
    for(int i = 0; i < set->count; i++){
      if(set->hasher(data) == set->hasher(temp->data)){
        if(prev==NULL){
          set->head = temp->next;
        }
        else{
          prev->next = temp->next;
        }
        free(temp);
        set->count--;
        return true;
      }
      prev = temp;
      temp = temp->next;
    }
    return false;
}

bool set_contains(set_t* set, void* data)
{
    if(set==NULL||set->count==0){
      return false;
    }
    set_item_t * temp = set->head;
    for(int i = 0; i < set->count; i++){
      if(set->hasher(data) == set->hasher(temp->data)){
        return true;
      }
      temp = temp->next;
    }
    return false;
}

size_t set_count(set_t* set)
{
    if(set==NULL){
      return 0;
    }
    return set->count;
}

void set_for_each(set_t* set, iterator_f iterator)
{
    if(NULL==set||set->count==0){
      return;
    }
    set_item_t *curr;
    for(curr = set->head; curr!=NULL; curr = curr->next){
      iterator(curr->data);
    }
    return;
}

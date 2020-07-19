// hashmap.c
// Generic hashmap data structure.

#include <stdlib.h>

#include "hashmap.h"

hashmap_t* hashmap_new(
    hasher_f hasher, 
    comparator_f comparator, 
    deleter_f deleter)
{
    return NULL;
}

void hashmap_delete(hashmap_t* map)
{
    return;
}

bool hashmap_insert(
    hashmap_t* map, 
    void* key, 
    void* value, 
    void** out)
{
    return false;
}

bool hashmap_remove(hashmap_t* map, void* key)
{
    return false;
}   

void* hashmap_find(hashmap_t* map, void* key)
{
    return NULL;
}

size_t hashmap_count(hashmap_t* map)
{
    return 0;
}
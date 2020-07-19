// hashmap.c
// Generic hashmap data structure.

#include <stdlib.h>

#include "hashmap.h"
#include "../intrusive-list/intrusive_list.h"

#define INIT_N_BUCKETS 16

// ----------------------------------------------------------------------------
// Internal Declarations

// Bins in our hashtable will simply be intrusive list headers.
typedef list_entry_t bucket_t;

static void initialize_table(bucket_t* buckets, size_t n);

// ----------------------------------------------------------------------------
// Exported

hashmap_t* hashmap_new(
    hasher_f hasher, 
    comparator_f comparator, 
    deleter_f deleter)
{
    if (NULL == hasher 
     || NULL == comparator 
     || NULL == deleter)
    {
        return NULL;
    }

    hashmap_t* map = malloc(sizeof(hashmap_t));
    if (NULL == map)
    {
        return NULL;
    }

    bucket_t* buckets = calloc(INIT_N_BUCKETS, sizeof(bucket_t));
    if (NULL == buckets)
    {
        free(map);
        return NULL;
    }

    initialize_table(buckets, INIT_N_BUCKETS);

    map->hasher     = hasher;
    map->comparator = comparator;
    map->deleter    = deleter;

    map->count = 0;

    return map;
}

void hashmap_delete(hashmap_t* map)
{
    if (NULL == map)
    {
        return;
    }
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

// ----------------------------------------------------------------------------
// Internal

static void initialize_table(bucket_t* buckets, size_t n)
{
    for (size_t i = 0; i < n; ++i)
    {
        list_init(&buckets[i]);
    }
}
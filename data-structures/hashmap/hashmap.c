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

typedef struct bucket_item
{
    // The embedded intrusive list entry
    list_entry_t entry;

    // Key and value pointers
    void* key;
    void* value;

    // Memoize the hash, not strictly necessary, but saves during resize.
    hash_t hash;
} bucket_item_t;

static void initialize_table(bucket_t* buckets, size_t n);

static void flush_bucket(bucket_t* bucket_head, deleter_f deleter);

static bucket_item_t* new_bucket_item(void* key, void* value, hash_t hash);
static void delete_bucket_item(bucket_item_t* item);

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

    map->count     = 0;
    map->n_buckets = INIT_N_BUCKETS;

    return map;
}

void hashmap_delete(hashmap_t* map)
{
    if (NULL == map)
    {
        return;
    }

    for (size_t i = 0; i < map->n_buckets; ++i)
    {
        flush_bucket(&(map->buckets[i]), map->deleter);
    }

    free(map);
}

bool hashmap_insert(
    hashmap_t* map, 
    void* key, 
    void* value, 
    void** out)
{
    if (NULL == map)
    {
        return false;
    } 

    *out = NULL;

    return true;
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
    return (NULL == map) ? 0 : map->count;
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

static void flush_bucket(bucket_t* bucket_head, deleter_f deleter)
{
    bucket_item_t* item;
    while ((item = (bucket_item_t*) list_pop_front(bucket_head)) != NULL)
    {
        deleter(item->value);
        delete_bucket_item(item);
    }
}

static bucket_item_t* new_bucket_item(void* key, void* value, hash_t hash)
{
    bucket_item_t* item = malloc(sizeof(bucket_item_t));
    if (NULL == item)
    {
        return NULL;
    }

    item->key   = key;
    item->value = value;
    item->hash  = hash;

    return item;
}

static void delete_bucket_item(bucket_item_t* item)
{
    free(item);
}

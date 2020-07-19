// hashmap.c
// Generic hashmap data structure.

#include <stdlib.h>

#include "hashmap.h"
#include "../intrusive-list/intrusive_list.h"

// The initial number of buckets in the internal table.
static const size_t INIT_N_BUCKETS = 16;

// The table load factor.
static const float LOAD_FACTOR = 0.75f;

// ----------------------------------------------------------------------------
// Internal Declarations

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

static void flush_bucket(
    bucket_t* bucket_head, 
    deleter_f deleter);

static bucket_item_t* new_bucket_item(
    void* key, 
    void* value, 
    hash_t hash);
static void delete_bucket_item(bucket_item_t* item);

static size_t index_for_hash(hash_t hash, size_t n_buckets);
static bucket_t* bucket_at_index(bucket_t* buckets, size_t index);

static bucket_item_t* find_in_bucket(
    bucket_t* bucket_head, 
    void* key, 
    comparator_f comparator);
static void* replace_in_bucket(
    bucket_t* bucket_head, 
    void* key, 
    void* value, 
    comparator_f comparator);
static void remove_from_bucket(
    bucket_t* bucket_head, 
    bucket_item_t* item);

static bool resize_required(size_t n_items, size_t n_buckets);
static bool resize_table(hashmap_t* map);

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

    map->buckets = buckets;

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

    if (out != NULL)
    {
        *out = NULL;
    }

    // compute the hash and index
    hash_t hash  = map->hasher(key);
    size_t index = index_for_hash(hash, map->n_buckets);

    // locate the associated bucket
    bucket_t* bucket = bucket_at_index(map->buckets, index);

    // searches the bucket for the specified key
    // replaces the value associated with key if found 
    // and returns old value otherwise returns NULL
    void* replaced_value = replace_in_bucket(
        bucket, 
        key, 
        value, 
        map->comparator);

    if (replaced_value != NULL)
    {
        // replaced a duplicate key, done
        if (out != NULL)
        {
            *out = replaced_value;
        }
    }
    else
    {
        // otherwise, need to perform the insertion
        bucket_item_t* item = new_bucket_item(key, value, hash);
        if (NULL == item)
        {
            return false;
        }

        list_push_front(bucket, &item->entry);
        map->count++;
    }

    // determine if a resize is triggered by this insertion;
    // someone with an eye for optimization might skip this
    // check in the event that the value was simply replaced
    if (resize_required(map->count, map->n_buckets))
    {
        resize_table(map);
    }

    return true;
}

bool hashmap_remove(hashmap_t* map, void* key)
{
    if (NULL == map)
    {
        return false;
    }

    // compute the hash and index
    hash_t hash  = map->hasher(key);
    size_t index = index_for_hash(hash, map->n_buckets);

    // locate the associated bucket
    bucket_t* bucket = bucket_at_index(map->buckets, index);

    // attempts to find the key in the given bucket;
    // returns pointer to bucket item if found, NULL otherwise
    bucket_item_t* item = find_in_bucket(
        bucket, 
        key, 
        map->comparator);

    const bool removed = item != NULL;
    if (removed)
    {
        remove_from_bucket(bucket, item);
        map->deleter(item->value);
        delete_bucket_item(item);

        map->count--;
    }

    return removed;
}   

void* hashmap_find(hashmap_t* map, void* key)
{
    if (NULL == map)
    {
        return NULL;
    }

    // compute the hash and index
    hash_t hash  = map->hasher(key);
    size_t index = index_for_hash(hash, map->n_buckets);

    // locate the associated bucket
    bucket_t* bucket = bucket_at_index(map->buckets, index);

    bucket_item_t* item = find_in_bucket(
        bucket, 
        key, 
        map->comparator);

    return NULL == item ? NULL : item->value;
}

size_t hashmap_count(hashmap_t* map)
{
    return (NULL == map) ? 0 : map->count;
}

// ----------------------------------------------------------------------------
// Internal

// initialize the internal table
static void initialize_table(bucket_t* buckets, size_t n)
{
    for (size_t i = 0; i < n; ++i)
    {
        list_init(&buckets[i]);
    }
}

// flush and destroy the contents of a bucket
static void flush_bucket(
    bucket_t* bucket_head, 
    deleter_f deleter)
{
    bucket_item_t* item;
    while ((item = (bucket_item_t*) list_pop_front(bucket_head)) != NULL)
    {
        deleter(item->value);
        delete_bucket_item(item);
    }
}

// construct a new bucket item
static bucket_item_t* new_bucket_item(
    void* key, 
    void* value, 
    hash_t hash)
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

// destroy a bucket item
static void delete_bucket_item(bucket_item_t* item)
{
    free(item);
}

// compute the bucket index for the given hash value
__always_inline
static size_t index_for_hash(hash_t hash, size_t n_buckets)
{
    return hash % n_buckets;
}

// return a reference to the bucket at the specified index in bucket array
__always_inline
static bucket_t* bucket_at_index(bucket_t* buckets, size_t index)
{
    return &buckets[index];
}

// searches the bucket for the specified key
// returns the matching bucket item if found, or NULL
static bucket_item_t* find_in_bucket(
    bucket_t* bucket_head, 
    void* key, 
    comparator_f comparator)
{
    bucket_item_t* found_item = NULL;

    list_entry_t* current = (list_entry_t*) bucket_head->flink;
    while (current != bucket_head)
    {
        bucket_item_t* item = (bucket_item_t*) current;
        if (comparator(item->key, key))
        {
            found_item = item;
            break;
        }
    }

    return found_item;
}

// searches the specified bucket for the specified key,
// replacing the value associated with key with `new_value`
// if found, returning the old value in this case, NULL otherwise
static void* replace_in_bucket(
    bucket_t* bucket_head, 
    void* key, 
    void* new_value, 
    comparator_f comparator)
{
    void* value = NULL;

    list_entry_t* current = (list_entry_t*) bucket_head->flink;
    while (current != bucket_head)
    {
        bucket_item_t* item = (bucket_item_t*) current;
        if (comparator(item->key, key))
        {
            value = item->value;
            item->value = new_value;
            break;
        }
    }

    return value;
}

// remove the specified bucket item from the bucket
static void remove_from_bucket(
    bucket_t* bucket_head, 
    bucket_item_t* item)
{
    // NOTE: this is hacky, in a perfect world
    // the intrusive_list API would support a remove
    // operation that would handle this low-level
    // linked-list manipulation, but I don't want to
    // alter the API now to suit this use case

    list_entry_t* entry = &item->entry;

    entry->flink->blink = entry->blink;
    entry->blink->flink = entry->flink;
}

// determine if a table resize is required
__always_inline
static bool resize_required(size_t n_items, size_t n_buckets)
{
    return n_items >= n_buckets*LOAD_FACTOR; 
}

// resizes the internal table
static bool resize_table(hashmap_t* map)
{
    // simple resize scheme: double table size
    const size_t new_size = map->n_buckets*2;

    bucket_t* new_buckets = calloc(new_size, sizeof(bucket_t));
    if (NULL == new_buckets)
    {
        return false;
    }

    initialize_table(new_buckets, new_size);

    // for each bucket
    for (size_t i = 0; i < map->n_buckets; ++i)
    {
        bucket_t* bucket = &map->buckets[i];

        // pop all items from the bucket
        bucket_item_t* item;
        while ((item = (bucket_item_t*)list_pop_front(bucket)) != NULL)
        {
            // and re-insert them into the new table
            size_t index = index_for_hash(item->hash, new_size);
            list_push_front(&new_buckets[index], &item->entry);
        }
    }

    // finally, update map to reflect resized table
    map->buckets   = new_buckets;
    map->n_buckets = new_size;

    return true;
}
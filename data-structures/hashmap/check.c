// check.c
// Driver program for stack data structure tests.

#include <check.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"
#include "intrusive_list.h"

// ----------------------------------------------------------------------------
// Definitions for Testing: Intrusive List

typedef struct intrusive_point
{
    list_entry_t entry;

    float x;
    float y;
} intrusive_point_t;

static intrusive_point_t* make_intrusive_point(float x, float y)
{
    intrusive_point_t* p = malloc(sizeof(intrusive_point_t));
    p->x = x;
    p->y = y;

    return p;
}

static void delete_intrusive_point(intrusive_point_t* p)
{
    free(p);
}

// ----------------------------------------------------------------------------
// Definitions for Testing: Hashmap 

typedef struct point
{
    uint64_t x;
    uint64_t y;
} point_t;

static point_t* make_point(uint64_t x, uint64_t y)
{
    point_t* p = malloc(sizeof(point_t));
    p->x = x;
    p->y = y;
    
    return p;
}

// The delete function provided for the point type.
static void delete_point(void* p)
{
    point_t* as_point = (point_t*)p;
    free(as_point);
}

// The hash function provided for the key type (string).
static hash_t hash_key(void* ptr)
{
    char* as_str = (char*)ptr;
    size_t len = strlen(as_str);

    hash_t hash = 0;
    for (size_t i = 0; i < len; ++i)
    {
        hash += (unsigned char) as_str[i];
    }

    return hash;
}

// The comparison function provided for the key type (string).
static bool compare_keys(void* p1, void* p2)
{
    char* as_str1 = (char*)p1;
    char* as_str2 = (char*)p2;
    return 0 == strcmp(as_str1, as_str2);
}

// ----------------------------------------------------------------------------
// Test Cases: Intrusive List

START_TEST(test_intrusive_list)
{
    const size_t N_ADDED = 5;

    // the head of the intrusive list
    intrusive_point_t points_head;

    points_head.x = 0.0f;
    points_head.y = 0.0f;

    // initialize the list head
    list_init(&points_head.entry);

    // push N_ADDED items onto the list
    for (size_t i = 0; i < N_ADDED; ++i)
    {
        float location = (i + 1)*1.0f;
        intrusive_point_t* p = make_intrusive_point(location, location);

        list_push_front(&points_head.entry, &p->entry);
    }

    const size_t count = list_count(&points_head.entry);
    ck_assert_msg(count == 5, "list_count() returned incorrect element count");

    // pop all the items off the list
    for (size_t i = 0; i < N_ADDED; ++i)
    {
        list_entry_t* entry = list_pop_back(&points_head.entry);
        ck_assert_msg(entry != NULL, "list_pop_back() unexpectedly returned NULL");

        intrusive_point_t* p = (intrusive_point_t*)entry;

        const float expected = (i + 1)*1.0f;

        const float x = p->x;
        const float y = p->y;

        ck_assert(x == expected);
        ck_assert(y == expected);

        delete_intrusive_point(p);
    }
}
END_TEST

// ----------------------------------------------------------------------------
// Test Cases: Hashmap

START_TEST(test_hashmap_new)
{
    hashmap_t* map1 = hashmap_new(NULL, NULL, NULL);
    ck_assert_msg(NULL == map1, "hashmap_new() returned non-NULL on invalid input");

    hashmap_t* map2 = hashmap_new(hash_key, compare_keys, delete_point);
    ck_assert_msg(map2 != NULL, "hashmap_new() returned NULL");

    ck_assert_msg(hashmap_count(map2) == 0, "hashmap_count() return nonzero on empty map");

    hashmap_delete(map2);
}
END_TEST

START_TEST(test_hashmap_insert)
{
    hashmap_t* map = hashmap_new(hash_key, compare_keys, delete_point);
    ck_assert_msg(map != NULL, "hashmap_new() returned NULL");

    ck_assert_msg(hashmap_count(map) == 0, "hashmap_count() returned incorrect count");

    void* out1 = NULL;
    void* out2 = NULL;
    void* out3 = NULL;

    point_t* p1 = make_point(1, 1);
    point_t* p2 = make_point(2, 2);
    point_t* p3 = make_point(3, 3);

    // first insertion, should succeed and out1 should be NULL
    const bool i1 = hashmap_insert(map, "one", p1, &out1);
    ck_assert_msg(i1, "hashmap_insert() returned false");
    ck_assert_msg(NULL == out1, "hashmap_insert() spuriously set out");

    ck_assert_msg(hashmap_count(map) == 1, "hashmap_count() returned incorrect count");

    // second insertion, same story as first
    const bool i2 = hashmap_insert(map, "two", p2, &out2);
    ck_assert_msg(i2, "hashmap_insert() returned false");
    ck_assert_msg(NULL == out2, "hashmap_insert() spuriously set out");

    ck_assert_msg(hashmap_count(map) == 2, "hashmap_count() returned incorrect count");

    // third insertion we insert with duplicate key
    // insertion should succeed and out should be set to p1
    const bool i3 = hashmap_insert(map, "one", p3, &out3);
    ck_assert_msg(i3, "hashmap_insert() returned false");
    ck_assert_msg(out3 != NULL, "hashmap_insert() did not set out to non-NULL");

    ck_assert_msg(hashmap_count(map) == 2, "hashmap_count() returned incorrect count");

    point_t* ret = (point_t*)out3;
    ck_assert(ret->x == 1);
    ck_assert(ret->y == 1);
    delete_point(ret);

    hashmap_delete(map);
}
END_TEST

START_TEST(test_hashmap_remove)
{
    hashmap_t* map = hashmap_new(hash_key, compare_keys, delete_point);
    ck_assert_msg(map != NULL, "hashmap_new() returned NULL");

    ck_assert_msg(hashmap_count(map) == 0, "hashmap_count() returned incorrect count");

    void* out1 = NULL;
    void* out2 = NULL;
    void* out3 = NULL;

    point_t* p1 = make_point(1, 1);
    point_t* p2 = make_point(2, 2);
    point_t* p3 = make_point(3, 3);

    ck_assert_msg(hashmap_insert(map, "one", p1, &out1), "hashmap_insert() failed");
    ck_assert_msg(hashmap_insert(map, "two", p2, &out2), "hashmap_insert() failed");
    ck_assert_msg(hashmap_insert(map, "six", p3, &out3), "hashmap_insert() failed");

    ck_assert(NULL == out1);
    ck_assert(NULL == out2);
    ck_assert(NULL == out3);

    ck_assert_msg(hashmap_count(map) == 3, "hashmap_count() returned incorrect count");

    ck_assert_msg(hashmap_remove(map, "one"), "hashmap_remove() failed on key present in map");
    ck_assert_msg(hashmap_remove(map, "two"), "hashmap_remove() failed on key present in map");

    ck_assert_msg(hashmap_count(map) == 1, "hashmap_count() returned incorrect count");

    ck_assert_msg(!hashmap_remove(map, "eleven"), "hashmap_remove() succeeded on key not present in map");

    point_t* ret = hashmap_find(map, "six");

    ck_assert_msg(ret != NULL, "hashmap_find() returned NULL for key present in map");

    ck_assert(ret->x == 3);
    ck_assert(ret->y == 3);

    hashmap_delete(map);
}
END_TEST

// ----------------------------------------------------------------------------
// Infrastructure
 
Suite* intrusive_list_suite(void)
{
    Suite* s = suite_create("intrusive-list");
    TCase* tc_core = tcase_create("intrusive-list-core");
    
    tcase_add_test(tc_core, test_intrusive_list);
    
    suite_add_tcase(s, tc_core);
    
    return s;
}

Suite* hashmap_suite(void)
{
    Suite* s = suite_create("hashmap");
    TCase* tc_core = tcase_create("hashmap-core");
    
    tcase_add_test(tc_core, test_hashmap_new);
    tcase_add_test(tc_core, test_hashmap_insert);
    tcase_add_test(tc_core, test_hashmap_remove);
    
    suite_add_tcase(s, tc_core);
    
    return s;
}

int main(void)
{
    Suite* list_suit = intrusive_list_suite();
    Suite* map_suite = hashmap_suite();

    SRunner* runner = srunner_create(list_suit);
    srunner_add_suite(runner, map_suite);

    srunner_run_all(runner, CK_NORMAL);    
    srunner_free(runner);
    
    return EXIT_SUCCESS;
}
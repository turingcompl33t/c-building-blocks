// check.c
// Driver program for set data structure tests.

// attribute gnu_printf
#pragma GCC diagnostic ignored "-Wignored-attributes"

#include <check.h>
#include <stdlib.h>

#include "set.h"

#define N_ADDED 5

// ----------------------------------------------------------------------------
// Definitions for Testing

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

// delete function given to set
static void delete_point(void* p)
{
    free(p);
}

// hash function given to set
static hash_t hash_point(void* p)
{
    // hash function incredibly simple;
    // uses first member of struct as hash
    point_t* as_point = (point_t*)p;
    return as_point->x;
}

static void increment_point(void* p)
{
    point_t* as_point = (point_t*)p;
    as_point->x++;
    as_point->y++;
}

// ----------------------------------------------------------------------------
// Test Cases

START_TEST(test_set_new)
{
    // new set construction should succeed
    set_t* s = set_new(delete_point, hash_point);
    ck_assert_msg(s != NULL, "set_new() returned NULL");

    // new set should be empty
    const size_t count = set_count(s);
    ck_assert_msg(count == 0, "newly constructed set nonempty");

    set_delete(s);
}
END_TEST

START_TEST(test_set_add_remove)
{
    point_t* points[N_ADDED];

    // new set construction should succeed
    set_t* s = set_new(delete_point, hash_point);
    ck_assert_msg(s != NULL, "set_new() returned NULL");

    // test adding new items to set
    for (size_t i = 0; i < N_ADDED; ++i)
    {
        point_t* p = make_point(i, i);
        const bool added = set_add(s, p);

        ck_assert_msg(added, "set_add() failed with unique item");

        points[i] = p;
    }

    // test find operations
    for (size_t i = 0; i < N_ADDED; ++i)
    {
        point_t* p = points[i];
        const bool found = set_contains(s, p);
        ck_assert_msg(found, "set_contains() returned false for data added to set");
    }

    // test remove operations
    for (size_t i = 0; i < N_ADDED; ++i)
    {
        point_t* p = points[i];
        const bool removed = set_remove(s, p);
        
        ck_assert_msg(removed, "set_remove() returned false for data added to set");

        free(p);
    }

    set_delete(s);
}
END_TEST

START_TEST(test_set_for_each)
{
    point_t* points[N_ADDED];

    // new set construction should succeed
    set_t* s = set_new(delete_point, hash_point);
    ck_assert_msg(s != NULL, "set_new() returned NULL");

    // add some items to the set
    for (size_t i = 0; i < N_ADDED; ++i)
    {
        point_t* p = make_point(i, i);
        const bool added = set_add(s, p);

        ck_assert_msg(added, "set_add() failed with unique item");

        points[i] = p;
    }
    
    // iterate over the set, with side effects
    set_for_each(s, increment_point);

    // make sure the side effects are observed
    for (size_t i = 0; i < N_ADDED; ++i)
    {
        point_t* p = points[i];
        const bool removed = set_remove(s, p);

        ck_assert_msg(removed, "set_remove() failed with added item");

        // make sure the data is as expected
        ck_assert(p->x == i + 1);
        ck_assert(p->y == i + 1);

        free(p);
    }

    set_delete(s);
}
END_TEST

// ----------------------------------------------------------------------------
// Infrastructure
 
Suite* stack_suite(void)
{
    Suite* s = suite_create("set");
    TCase* tc_core = tcase_create("set-core");
    
    tcase_add_test(tc_core, test_set_new);
    tcase_add_test(tc_core, test_set_add_remove);
    tcase_add_test(tc_core, test_set_for_each);

    suite_add_tcase(s, tc_core);
    
    return s;
}

int main(void)
{
    Suite* suite = stack_suite();
    SRunner* runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);    
    srunner_free(runner);
    
    return EXIT_SUCCESS;
}
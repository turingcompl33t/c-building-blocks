// check.c
// Driver program for array data structure tests.

// attribute gnu_printf
#pragma GCC diagnostic ignored "-Wignored-attributes"

#include <check.h>
#include <stdlib.h>

#include "array.h"

// ----------------------------------------------------------------------------
// Definitions for Testing

typedef struct point
{
    float x;
    float y;
} point_t;

static point_t* make_point(float x, float y)
{
    point_t* p = malloc(sizeof(point_t));
    p->x = x;
    p->y = y;

    return p;
}

static void delete_point(void* p)
{
    point_t* as_point = (point_t*)p;
    free(as_point);
}

// ----------------------------------------------------------------------------
// Test Cases

START_TEST(test_array_new)
{
    array_t* a1 = array_new(5, delete_point);
    ck_assert(a1 != NULL);
    ck_assert(array_capacity(a1) == 8);
    ck_assert(array_count(a1) == 0);

    array_t* a2 = array_new(60, delete_point);
    ck_assert(a2 != NULL);
    ck_assert(array_capacity(a2) == 64);
    ck_assert(array_count(a2) == 0);

    array_delete(a1);
    array_delete(a2);
}
END_TEST

START_TEST(test_array_put_get_0)
{
    array_t* array = array_new(8, delete_point);
    ck_assert(array != NULL);
    ck_assert(array_capacity(array) == 8);

    point_t* p1 = make_point(1.0f, 1.0f);
    array_put(array, 6, p1);
    ck_assert(array_capacity(array) == 8);
    ck_assert(array_count(array) == 1);

    point_t* p2 = make_point(2.0f, 2.0f);
    array_put(array, 9, p2);
    ck_assert(array_capacity(array) == 16);
    ck_assert(array_count(array) == 2);

    point_t* p1_out = (point_t*) array_get(array, 6);
    ck_assert(p1_out != NULL);
    ck_assert(p1_out->x == 1.0f);
    ck_assert(p1_out->y == 1.0f);

    point_t* p2_out = (point_t*) array_get(array, 9);
    ck_assert(p2_out != NULL);
    ck_assert(p2_out->x == 2.0f);
    ck_assert(p2_out->y == 2.0f);

    array_delete(array);
}
END_TEST

START_TEST(test_array_put_get_1)
{
    array_t* array = array_new(8, delete_point);
    ck_assert(array != NULL);
    ck_assert(array_capacity(array) == 8);

    point_t* p1 = make_point(1.0f, 1.0f);
    array_put(array, 5, p1);
    ck_assert(array_capacity(array) == 8);
    ck_assert(array_count(array) == 1);

    point_t* p2 = make_point(2.0f, 2.0f);
    array_put(array, 5, p2);
    ck_assert(array_capacity(array) == 8);
    ck_assert(array_count(array) == 1);

    point_t* p2_out = array_get(array, 5);
    ck_assert(p2_out != NULL);
    ck_assert(p2_out->x == 2.0f);
    ck_assert(p2_out->y == 2.0f);

    array_delete(array);
}
END_TEST

START_TEST(test_array_put_erase_0)
{
    array_t* array = array_new(8, delete_point);
    ck_assert(array != NULL);
    ck_assert(array_capacity(array) == 8);

    // insert two points, triggering resize

    point_t* p1 = make_point(1.0f, 1.0f);
    array_put(array, 6, p1);
    ck_assert(array_capacity(array) == 8);
    ck_assert(array_count(array) == 1);

    point_t* p2 = make_point(2.0f, 2.0f);
    array_put(array, 9, p2);
    ck_assert(array_capacity(array) == 16);
    ck_assert(array_count(array) == 2);

    // ensure the contents are as expected

    point_t* p1_out = (point_t*) array_get(array, 6);
    ck_assert(p1_out != NULL);
    ck_assert(p1_out->x == 1.0f);
    ck_assert(p1_out->y == 1.0f);

    point_t* p2_out = (point_t*) array_get(array, 9);
    ck_assert(p2_out != NULL);
    ck_assert(p2_out->x == 2.0f);
    ck_assert(p2_out->y == 2.0f);

    // erase a point, triggering another resize

    array_erase(array, 9);
    ck_assert(array_capacity(array) == 8);
    ck_assert(array_count(array) == 1);

    // second erasure should not trigger resize
    // because of specified minimum capacity of 8

    array_erase(array, 6);
    ck_assert(array_capacity(array) == 8);
    ck_assert(array_count(array) == 0);

    ck_assert(NULL == array_get(array, 6));
    ck_assert(NULL == array_get(array, 9));

    array_delete(array);
}
END_TEST

START_TEST(test_array_put_erase_1)
{
    array_t* array = array_new(8, delete_point);
    ck_assert(array != NULL);
    ck_assert(array_capacity(array) == 8);

    // insert a point, no resize

    point_t* p1 = make_point(1.0f, 1.0f);
    array_put(array, 0, p1);
    ck_assert(array_capacity(array) == 8);
    ck_assert(array_count(array) == 1);

    // ensure the contents are as expected

    point_t* p1_out = (point_t*) array_get(array, 0);
    ck_assert(p1_out != NULL);
    ck_assert(p1_out->x == 1.0f);
    ck_assert(p1_out->y == 1.0f);

    // erase the point

    array_erase(array, 0);
    ck_assert(array_capacity(array) == 8);
    ck_assert(array_count(array) == 0);

    array_delete(array);
}
END_TEST

// ----------------------------------------------------------------------------
// Infrastructure
 
Suite* array_suite(void)
{
    Suite* s = suite_create("array");
    TCase* tc_core = tcase_create("array-core");
    
    tcase_add_test(tc_core, test_array_new);
    tcase_add_test(tc_core, test_array_put_get_0);
    tcase_add_test(tc_core, test_array_put_get_1);
    tcase_add_test(tc_core, test_array_put_erase_0);
    tcase_add_test(tc_core, test_array_put_erase_1);

    suite_add_tcase(s, tc_core);
    
    return s;
}

int main(void)
{
    Suite* suite = array_suite();
    SRunner* runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);    
    srunner_free(runner);
    
    return EXIT_SUCCESS;
}
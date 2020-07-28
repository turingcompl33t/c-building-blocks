// check.c
// Driver program for ring buffer data structure tests.

// attribute gnu_printf
#pragma GCC diagnostic ignored "-Wignored-attributes"

#include <check.h>
#include <stdlib.h>

#include "ring_buffer.h"

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

START_TEST(test_ring_buffer_new)
{
    ring_buffer_t* buffer = buffer_new(8, delete_point);
    ck_assert(buffer != NULL);

    buffer_delete(buffer);
}
END_TEST

START_TEST(test_ring_buffer_put)
{
    ring_buffer_t* buffer = buffer_new(2, delete_point);
    ck_assert(buffer != NULL);

    point_t* p1 = make_point(1.0f, 1.0f);
    point_t* p2 = make_point(2.0f, 2.0f);
    point_t* p3 = make_point(3.0f, 3.0f);

    ck_assert(buffer_put(buffer, p1));
    ck_assert(buffer_put(buffer, p2));
    ck_assert(!buffer_put(buffer, p3));

    void* out = NULL;

    ck_assert(buffer_get(buffer, &out));
    
    point_t* as_point = (point_t*)out;
    ck_assert(as_point->x == 1.0f);
    ck_assert(as_point->y == 1.0f);
    delete_point(out);

    ck_assert(buffer_put(buffer, p3));

    buffer_delete(buffer);
}
END_TEST

// ----------------------------------------------------------------------------
// Infrastructure
 
Suite* ring_buffer_suite(void)
{
    Suite* s = suite_create("ring-buffer");
    TCase* tc_core = tcase_create("ring-buffer-core");
    
    tcase_add_test(tc_core, test_ring_buffer_new);
    tcase_add_test(tc_core, test_ring_buffer_put);

    suite_add_tcase(s, tc_core);
    
    return s;
}

int main(void)
{
    Suite* suite = ring_buffer_suite();
    SRunner* runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);    
    srunner_free(runner);
    
    return EXIT_SUCCESS;
}
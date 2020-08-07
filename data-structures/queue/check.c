// check.c
// Driver program for queue data structure tests.

// attribute gnu_printf
#pragma GCC diagnostic ignored "-Wignored-attributes"

#include <check.h>
#include <stdlib.h>

#include "queue.h"

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

static void delete_point(point_t* p)
{
    free(p);
}

static void point_deleter(void* p)
{
    point_t* as_p = (point_t*) p;
    free(as_p);
}

// ----------------------------------------------------------------------------
// Test Cases

START_TEST(test_queue_0)
{
    queue_t* queue = queue_new(point_deleter);
    ck_assert_msg(queue != NULL, "queue_new() returned NULL");

    point_t* p1 = make_point(1.0, 1.0);
    point_t* p2 = make_point(2.0, 2.0);
    point_t* p3 = make_point(3.0, 3.0);
    point_t* p4 = make_point(4.0, 4.0);

    ck_assert(queue_push(queue, p1));
    ck_assert(queue_push(queue, p2));
    ck_assert(queue_push(queue, p3));
    ck_assert(queue_push(queue, p4));

    point_t* out1 = (point_t*)queue_pop(queue);
    ck_assert(out1 != NULL);
    ck_assert(out1->x == 1.0);
    ck_assert(out1->y == 1.0);
    delete_point(out1);

    point_t* out2 = (point_t*)queue_pop(queue);
    ck_assert(out2 != NULL);
    ck_assert(out2->x == 2.0);
    ck_assert(out2->y == 2.0); 
    delete_point(out2);   
    
    point_t* out3 = (point_t*)queue_pop(queue);
    ck_assert(out3 != NULL);
    ck_assert(out3->x == 3.0);
    ck_assert(out3->y == 3.0);    
    delete_point(out3);

    point_t* out4 = (point_t*)queue_pop(queue);
    ck_assert(out4 != NULL);
    ck_assert(out4->x == 4.0);
    ck_assert(out4->y == 4.0);
    delete_point(out4);

    void* out5 = queue_pop(queue);
    ck_assert(NULL == out5);

    queue_delete(queue);
}
END_TEST

// ----------------------------------------------------------------------------
// Infrastructure
 
Suite* queue_suite(void)
{
    Suite* s = suite_create("queue");
    TCase* tc_core = tcase_create("queue-core");
    
    tcase_add_test(tc_core, test_queue_0);
    
    suite_add_tcase(s, tc_core);
    
    return s;
}

int main(void)
{
    Suite* suite = queue_suite();
    SRunner* runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);    
    srunner_free(runner);
    
    return EXIT_SUCCESS;
}
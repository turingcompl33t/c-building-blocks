// check.c
// Driver program for priority queue structure tests.

// attribute gnu_printf
#pragma GCC diagnostic ignored "-Wignored-attributes"

#include <check.h>
#include <stdlib.h>
#include <assert.h>

#include "priority_queue.h"

typedef struct point
{
    int x;
    int y;
} point_t;

static point_t* make_point(int x, int y)
{
    point_t* p = malloc(sizeof(point_t));
    assert(p != NULL);

    p->x = x;
    p->y = y;

    return p;
}

static void delete_point(point_t* p)
{
    free(p);
}

static priority_t point_prioritizer(void* p1, void* p2)
{
    point_t* as_p1 = (point_t*) p1;
    point_t* as_p2 = (point_t*) p2;

    const int sum_p1 = as_p1->x + as_p1->y;
    const int sum_p2 = as_p2->x + as_p2->y;

    if (sum_p1 > sum_p2)
    {
        return GREATER;
    }
    else if (sum_p1 == sum_p2)
    {
        return EQUAL;
    }
    else
    {
        return LESS;
    }
}

static void point_deleter(void* p)
{
    point_t* as_p = (point_t*) p;
    free(as_p);
}

// ----------------------------------------------------------------------------
// Test Cases

START_TEST(test_queue_new)
{
    priority_queue_t* queue = queue_new(point_prioritizer, point_deleter);
    ck_assert_msg(queue != NULL, "queue_new() returned NULL");

    queue_delete(queue);
}
END_TEST

START_TEST(test_queue_push_pop)
{
    priority_queue_t* queue = queue_new(point_prioritizer, point_deleter);
    ck_assert_msg(queue != NULL, "queue_new() returned NULL");

    point_t* p1 = make_point(1, 1);
    point_t* p2 = make_point(2, 2);
    point_t* p3 = make_point(3, 3);
    point_t* p4 = make_point(4, 4);

    ck_assert(queue_push(queue, p1));
    ck_assert(queue_push(queue, p2));
    ck_assert(queue_push(queue, p3));
    ck_assert(queue_push(queue, p4));

    // items should be popped in reverse order of
    // insertion based on given priority function

    void* out4 = queue_pop(queue);
    ck_assert(out4 != NULL);
    point_t* as_p4 = (point_t*)out4;
    ck_assert(as_p4->x == 4);
    ck_assert(as_p4->y == 4);
    delete_point(as_p4);

    void* out3 = queue_pop(queue);
    ck_assert(out3 != NULL);
    point_t* as_p3 = (point_t*)out3;
    ck_assert(as_p3->x == 3);
    ck_assert(as_p3->y == 3);
    delete_point(as_p3);

    void* out2 = queue_pop(queue);
    ck_assert(out2 != NULL);
    point_t* as_p2 = (point_t*)out2;
    ck_assert(as_p2->x == 2);
    ck_assert(as_p2->y == 2);
    delete_point(as_p2);

    void* out1 = queue_pop(queue);
    ck_assert(out1 != NULL);
    point_t* as_p1 = (point_t*)out1;
    ck_assert(as_p1->x == 1);
    ck_assert(as_p1->y == 1);
    delete_point(as_p1);

    // final pop should fail; empty queue

    void* out5 = queue_pop(queue);
    ck_assert(NULL == out5);

    queue_delete(queue);
}
END_TEST

// ----------------------------------------------------------------------------
// Infrastructure
 
Suite* priority_queue_suite(void)
{
    Suite* s = suite_create("priority-queue");
    TCase* tc_core = tcase_create("priority-queue-core");
    
    tcase_add_test(tc_core, test_queue_new);
    tcase_add_test(tc_core, test_queue_push_pop);
    
    suite_add_tcase(s, tc_core);
    
    return s;
}

int main(void)
{
    Suite* suite = priority_queue_suite();
    SRunner* runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);    
    srunner_free(runner);
    
    return EXIT_SUCCESS;
}
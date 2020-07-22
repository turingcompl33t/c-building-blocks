// check.c
// Driver program for stack data structure tests.

// attribute gnu_printf
#pragma GCC diagnostic ignored "-Wignored-attributes"

#include <check.h>
#include <stdlib.h>

#include "stack.h"

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

// ----------------------------------------------------------------------------
// Test Cases

START_TEST(test_stack_new)
{
    // new stack construction should succeed
    lstack_t* s = stack_new();
    ck_assert_msg(s != NULL, "stack_new() returned NULL");

    // new stack should be empty
    const size_t count = stack_count(s);
    ck_assert_msg(count == 0, "newly constructed stack nonempty");

    // destruction of stack should succeed because it is empty
    const bool success = stack_delete(s);
    ck_assert_msg(success, "stack_delete() on empty stack failed");
}
END_TEST

START_TEST(test_stack_delete)
{
    lstack_t* s = stack_new();
    ck_assert_msg(s != NULL, "stack_new() returned NULL");

    point_t* p = make_point(1.0f, 2.0f);
    const bool pushed1 = stack_push(s, p);
    ck_assert_msg(pushed1, "stack_push() failed");

    // attempt to delete stack should fail because stack nonempty
    const bool deleted1 = stack_delete(s);
    ck_assert_msg(!deleted1, "stack_delete() succeeded on nonempty stack");

    // remove the pushed item
    p = stack_pop(s);
    ck_assert_msg(p != NULL, "stack_pop() failed to return pushed item");

    // check the item's contents
    ck_assert(p->x == 1.0f);
    ck_assert(p->y ==2.0f);
    delete_point(p);

    // now delete should succeed
    const bool deleted2 = stack_delete(s);
    ck_assert_msg(deleted2, "stack_delete() on empty stack failed");
}
END_TEST

START_TEST(test_stack_push_pop)
{
    const size_t N_PUSHED = 5;
    
    lstack_t* s = stack_new();
    ck_assert_msg(s != NULL, "stack_new() returned NULL");

    for (size_t i = 0; i < N_PUSHED; ++i)
    {   
        float coordinates = i*1.0f;
        point_t* p = make_point(coordinates, coordinates);
        const bool pushed = stack_push(s, p);
        ck_assert_msg(pushed, "stack_push() failed");
    }

    for (size_t i = 0; i < N_PUSHED; ++i)
    {
        point_t* p = stack_pop(s);
        ck_assert_msg(p != NULL, "stack_pop() on nonempty stack returned NULL");

        // invert order of expected (last in, first out)
        const float expected = (N_PUSHED - 1 - i)*1.0f;

        float x = p->x;
        float y = p->y;

        ck_assert_msg(x == expected, "stack_pop() returned unexpected data");
        ck_assert_msg(y == expected, "stack_pop() returned unexpected data");
        delete_point(p);
    }

    const size_t count = stack_count(s);
    ck_assert_msg(count == 0, "stack_count() returned nonzero for expected empty stack");

    const bool deleted = stack_delete(s);
    ck_assert_msg(deleted, "stack_delete() on empty stack failed");
}
END_TEST

START_TEST(test_stack_peek)
{
    lstack_t* s = stack_new();
    ck_assert_msg(s != NULL, "stack_new() returned NULL");

    point_t* p = make_point(1.0f, 2.0f);
    const bool pushed = stack_push(s, p);
    ck_assert_msg(pushed, "stack_push() failed");

    // peek at the top of the stack
    p = stack_peek(s);
    ck_assert_msg(p != NULL, "stack_peek() returned NULL on nonempty stack");

    // check the peeked content
    ck_assert(p->x == 1.0f);
    ck_assert(p->y == 2.0f);

    // non remove the item
    p = stack_pop(s);
    ck_assert_msg(p != NULL, "stack_pop() returned NULL on nonempty stack");

    // check the removed content
    ck_assert(p->x == 1.0f);
    ck_assert(p->y == 2.0f);
    delete_point(p);

    // delete the stack
    const bool deleted = stack_delete(s);
    ck_assert_msg(deleted, "stack_delete() failed on empty stack");
}
END_TEST

// ----------------------------------------------------------------------------
// Infrastructure
 
Suite* stack_suite(void)
{
    Suite* s = suite_create("stack");
    TCase* tc_core = tcase_create("stack-core");
    
    tcase_add_test(tc_core, test_stack_new);
    tcase_add_test(tc_core, test_stack_delete);
    tcase_add_test(tc_core, test_stack_push_pop);
    tcase_add_test(tc_core, test_stack_peek);
    
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
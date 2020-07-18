// check.c
// Driver program for intrusive list data structure tests.

#include <check.h>
#include <stdlib.h>

#include "intrusive_list.h"

// ----------------------------------------------------------------------------
// Definitions for Testing

typedef struct point
{
    list_entry_t entry;

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

START_TEST(test_intrusive_list)
{
    const size_t N_ADDED = 5;

    // the head of the intrusive list
    point_t points_head;

    points_head.x = 0.0f;
    points_head.y = 0.0f;

    // initialize the list head
    list_init(&points_head.entry);

    // push N_ADDED items onto the list
    for (size_t i = 0; i < N_ADDED; ++i)
    {
        float location = (i + 1)*1.0f;
        point_t* p = make_point(location, location);

        list_push_front(&points_head.entry, &p->entry);
    }

    const size_t count = list_count(&points_head.entry);
    ck_assert_msg(count == 5, "list_count() returned incorrect element count");

    // pop all the items off the list
    for (size_t i = 0; i < N_ADDED; ++i)
    {
        list_entry_t* entry = list_pop_back(&points_head.entry);
        ck_assert_msg(entry != NULL, "list_pop_back() unexpectedly returned NULL");

        point_t* p = (point_t*)entry;

        const float expected = (i + 1)*1.0f;

        const float x = p->x;
        const float y = p->y;

        ck_assert(x == expected);
        ck_assert(y == expected);

        delete_point(p);
    }
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

int main(void)
{
    Suite* suite = intrusive_list_suite();
    SRunner* runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);    
    srunner_free(runner);
    
    return EXIT_SUCCESS;
}
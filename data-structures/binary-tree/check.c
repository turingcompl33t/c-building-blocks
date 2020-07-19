// check.c
// Driver program for stack data structure tests.

#include <check.h>
#include <stdlib.h>

#include "binary_tree.h"

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

static ordering_t compare_strings(void* a, void* b)
{
    char* a_str = (char*)a;
    char* b_str = (char*)b;

    const int res = strcmp(a_str, b_str);
    
    if (res < 0)
    {
        return LESS;
    }
    else if (res > 0)
    {
        return GREATER;
    }
    else
    {
        return EQUAL;
    }
}

// ----------------------------------------------------------------------------
// Test Cases

START_TEST(test_binary_tree_new)
{
    binary_tree_t* tree = tree_new(compare_strings, delete_point);
    ck_assert_msg(tree != NULL, "tree_new() returned NULL");

    const size_t count = tree_count(tree);
    ck_assert_msg(count == 0, "tree_count() returned nonzero for empty tree");

    tree_delete(tree);
}
END_TEST

START_TEST(test_binary_tree_insert_find)
{
	binary_tree_t* tree = tree_new(compare_strings, delete_point); 
    ck_assert_msg(tree != NULL, "tree_new() returned NULL");

    void* out1 = NULL;
    void* out2 = NULL;
    void* out3 = NULL;

	point_t* x = make_point(1.0f, 1.0f);
	point_t* y = make_point(2.0f, 2.0f);
	point_t* z = make_point(3.0f, 3.0f);

    point_t* x2 = NULL;
    point_t* y2 = NULL;
    point_t* z2 = NULL;

    // insert x under "one"
    const bool r1 = tree_insert(tree, "one", x, &out1);
    ck_assert_msg(r1, "tree_insert() failed unexpectedly");

    ck_assert_msg(tree_count(tree) == 1, "tree_count() returned incorrect count");

    x2 = tree_find(tree, "one");
    ck_assert_msg(x2 != NULL, "tree_find() failed unexpectedly");

    ck_assert(x2->x == 1.0f);
    ck_assert(x2->y == 1.0f);

    ck_assert(out1 == NULL);

    // insert y under "two"
    const bool r2 = tree_insert(tree, "two", y, &out2);
    ck_assert_msg(r2, "tree_insert() failed unexpectedly");

    ck_assert_msg(tree_count(tree) == 2, "tree_count() returned incorrect count");

    y2 = tree_find(tree, "two");
    ck_assert_msg(y2 != NULL, "tree_find() failed unexpectedly");

    ck_assert(y2->x == 2.0f);
    ck_assert(y2->y == 2.0f);

    ck_assert(out2 == NULL);

    // insert z under "one"
    const bool r3 = tree_insert(tree, "one", z, &out3);
    ck_assert_msg(r3, "tree_insert() failed unexpectedly");

    ck_assert_msg(tree_count(tree) == 2, "tree_count() returned incorrect count");

    ck_assert(out3 != NULL);

    // the data originally inserted under "one"
    point_t* ret = (point_t*) out3;
    ck_assert(ret->x == 1.0f);
    ck_assert(ret->y == 1.0f);
    delete_point(ret);

    z2 = tree_find(tree, "one");
    ck_assert_msg(z2 != NULL, "tree_find() failed unexpectedly");

    ck_assert(z2->x == 3.0f);
    ck_assert(z2->y == 3.0f);

	tree_delete(tree); 
}
END_TEST

// ----------------------------------------------------------------------------
// Infrastructure
 
Suite* binary_tree_suite(void)
{
    Suite* s = suite_create("binary-tree");
    TCase* tc_core = tcase_create("binary-tree-core");
    
    tcase_add_test(tc_core, test_binary_tree_new);
    tcase_add_test(tc_core, test_binary_tree_insert_find);
    
    suite_add_tcase(s, tc_core);
    
    return s;
}

int main(void)
{
    Suite* suite = binary_tree_suite();
    SRunner* runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);    
    srunner_free(runner);
    
    return EXIT_SUCCESS;
}
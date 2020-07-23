// check.c
// Driver program for stack data structure tests.

// attribute gnu_printf
#pragma GCC diagnostic ignored "-Wignored-attributes"

#include <check.h>
#include <stdlib.h>

#include "binary_tree.h"

// ----------------------------------------------------------------------------
// Definitions for Testing: Point

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
// Definitions for Testing: Key / Value

typedef struct key
{
    size_t k;
} key_t;

typedef struct value
{
    size_t v;
} value_t;

static key_t* make_key(const size_t k)
{
    key_t* key = malloc(sizeof(key_t));
    key->k = k;
    return key;
}

static void delete_key(void* key)
{
    key_t* as_key = (key_t*) key;
    free(as_key);
}

static value_t* make_value(const size_t v)
{
    value_t* value = malloc(sizeof(value_t));
    value->v = v;
    return value;
}

static void delete_value(void* value)
{
    value_t* as_value = (value_t*) value;
    free(as_value);
}

static ordering_t compare_keys(void* k1, void* k2)
{
    key_t* as_key1 = (key_t*) k1;
    key_t* as_key2 = (key_t*) k2;
    
    const size_t i1 = as_key1->k;
    const size_t i2 = as_key2->k;
    
    if (i1 < i2)
    {
        return LESS;
    }
    else if (i1 > i2)
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

START_TEST(test_binary_tree_remove_0)
{
	binary_tree_t* tree = tree_new(compare_keys, delete_value); 
    ck_assert_msg(tree != NULL, "tree_new() returned NULL");

    key_t* k1 = make_key(25);
    key_t* k2 = make_key(20);

    value_t* v1 = make_value(1);
    value_t* v2 = make_value(2);

	const bool r1 = tree_insert(tree, k1, v1, NULL);
	const bool r2 = tree_insert(tree, k2, v2, NULL); 

    ck_assert(r1);
    ck_assert(r2);

    const bool r3 = tree_remove(tree, k2);
    ck_assert(r3);

    delete_key(k1);
    tree_delete(tree);
}
END_TEST

START_TEST(test_binary_tree_remove_1)
{
	binary_tree_t* tree = tree_new(compare_keys, delete_value); 
    ck_assert_msg(tree != NULL, "tree_new() returned NULL");

    key_t* k1 = make_key(25);

    value_t* v1 = make_value(1);

	const bool r1 = tree_insert(tree, k1, v1, NULL);

    ck_assert(r1);

    const bool r3 = tree_remove(tree, k1);
    ck_assert(r3);

    delete_key(k1);
    tree_delete(tree);
}
END_TEST

START_TEST(test_binary_tree_remove_2)
{
	binary_tree_t* tree = tree_new(compare_keys, delete_value); 
    ck_assert_msg(tree != NULL, "tree_new() returned NULL");

    key_t* k1 = make_key(50);
    key_t* k2 = make_key(25);
    key_t* k3 = make_key(10);
    key_t* k4 = make_key(35);

    value_t* v1 = make_value(1);
    value_t* v2 = make_value(2);
    value_t* v3 = make_value(3);
    value_t* v4 = make_value(4);

	const bool r1 = tree_insert(tree, k1, v1, NULL);
	const bool r2 = tree_insert(tree, k2, v2, NULL); 
	const bool r3 = tree_insert(tree, k3, v3, NULL); 
	const bool r4 = tree_insert(tree, k4, v4, NULL); 

    ck_assert(r1);
    ck_assert(r2);
    ck_assert(r3);
    ck_assert(r4);

    const bool r5 = tree_remove(tree, k2);
    ck_assert(r5);

    delete_key(k2);
    tree_delete(tree);
}
END_TEST

START_TEST(test_binary_tree_remove_3)
{
	binary_tree_t* tree = tree_new(compare_keys, delete_value); 
    ck_assert_msg(tree != NULL, "tree_new() returned NULL");

    key_t* k1 = make_key(50);
    key_t* k2 = make_key(25);
    key_t* k3 = make_key(75);

    value_t* v1 = make_value(1);
    value_t* v2 = make_value(2);
    value_t* v3 = make_value(2);

	const bool r1 = tree_insert(tree, k1, v1, NULL);
	const bool r2 = tree_insert(tree, k2, v2, NULL);
	const bool r3 = tree_insert(tree, k3, v3, NULL);

    ck_assert(r1);
    ck_assert(r2);
    ck_assert(r3);

    const bool r4 = tree_remove(tree, k1);
    ck_assert(r4);

    delete_key(k1);
    tree_delete(tree);
}
END_TEST

START_TEST(test_binary_tree_remove_4)
{
	binary_tree_t* tree = tree_new(compare_keys, delete_value); 
    ck_assert_msg(tree != NULL, "tree_new() returned NULL");

    key_t* k1 = make_key(50);
    key_t* k2 = make_key(25);
    key_t* k3 = make_key(75);
    key_t* k4 = make_key(15);

    value_t* v1 = make_value(1);
    value_t* v2 = make_value(2);
    value_t* v3 = make_value(3);
    value_t* v4 = make_value(4);

	const bool r1 = tree_insert(tree, k1, v1, NULL);
	const bool r2 = tree_insert(tree, k2, v2, NULL);
	const bool r3 = tree_insert(tree, k3, v3, NULL);
	const bool r4 = tree_insert(tree, k4, v4, NULL);

    ck_assert(r1);
    ck_assert(r2);
    ck_assert(r3);
    ck_assert(r4);

    const bool r5 = tree_remove(tree, k4);
    ck_assert(r5);

    delete_key(k4);
    tree_delete(tree);
}
END_TEST

START_TEST(test_binary_tree_remove_5)
{
	binary_tree_t* tree = tree_new(compare_keys, delete_value); 
    ck_assert_msg(tree != NULL, "tree_new() returned NULL");

    key_t* k1 = make_key(50);
    key_t* k2 = make_key(25);

    value_t* v1 = make_value(1);
    value_t* v2 = make_value(2);

	const bool r1 = tree_insert(tree, k1, v1, NULL);
	const bool r2 = tree_insert(tree, k2, v2, NULL);

    ck_assert(r1);
    ck_assert(r2);

    const bool r3 = tree_remove(tree, k1);
    ck_assert(r3);

    delete_key(k1);
    tree_delete(tree);
}
END_TEST

START_TEST(test_binary_tree_remove_6)
{
	binary_tree_t* tree = tree_new(compare_keys, delete_value); 
    ck_assert_msg(tree != NULL, "tree_new() returned NULL");

    key_t* k1 = make_key(50);
    key_t* k2 = make_key(25);
    key_t* k3 = make_key(35);

    value_t* v1 = make_value(1);
    value_t* v2 = make_value(2);
    value_t* v3 = make_value(3);

	const bool r1 = tree_insert(tree, k1, v1, NULL);
	const bool r2 = tree_insert(tree, k2, v2, NULL);
	const bool r3 = tree_insert(tree, k3, v3, NULL);

    ck_assert(r1);
    ck_assert(r2);
    ck_assert(r3);

    const bool r4 = tree_remove(tree, k2);
    ck_assert(r4);

    delete_key(k2);
    tree_delete(tree);
}
END_TEST

START_TEST(test_binary_tree_remove_7)
{
	binary_tree_t* tree = tree_new(compare_keys, delete_value); 
    ck_assert_msg(tree != NULL, "tree_new() returned NULL");

    key_t* k1 = make_key(50);
    key_t* k2 = make_key(75);

    value_t* v1 = make_value(1);
    value_t* v2 = make_value(2);

	const bool r1 = tree_insert(tree, k1, v1, NULL);
	const bool r2 = tree_insert(tree, k2, v2, NULL);

    ck_assert(r1);
    ck_assert(r2);

    const bool r3 = tree_remove(tree, k1);
    ck_assert(r3);

    delete_key(k1);
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
    tcase_add_test(tc_core, test_binary_tree_remove_0);
    tcase_add_test(tc_core, test_binary_tree_remove_1);
    tcase_add_test(tc_core, test_binary_tree_remove_2);
    tcase_add_test(tc_core, test_binary_tree_remove_3);
    tcase_add_test(tc_core, test_binary_tree_remove_4);
    tcase_add_test(tc_core, test_binary_tree_remove_5);
    tcase_add_test(tc_core, test_binary_tree_remove_6);
    tcase_add_test(tc_core, test_binary_tree_remove_7);
    
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
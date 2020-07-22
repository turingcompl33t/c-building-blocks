// check.c
// Driver program for accumulate algorithm tests.

// attribute gnu_printf
#pragma GCC diagnostic ignored "-Wignored-attributes"

#include <check.h>
#include <stdlib.h>

#include "accumulate.h"

#define N_ITEMS 8

// ----------------------------------------------------------------------------
// Definitions for Testing

static int binary_add(int a, int b)
{
    return a + b;
}

static int binary_mul(int a, int b)
{
    return a * b;
}

static void initialize_array(int array[], size_t len)
{
    for (size_t i = 0; i < len; ++i)
    {
        array[i] = i + 1;
    }
}

// ----------------------------------------------------------------------------
// Test Cases

START_TEST(test_accumulate_add)
{
    int array[N_ITEMS];
    initialize_array(array, N_ITEMS);

    const int a1 = accumulate(array, 0, N_ITEMS, 0, binary_add);
    ck_assert_msg(a1 == 36, "accumulate() returned unexpected result for binary add");

    const int a2 = accumulate(array, 4, N_ITEMS, 0, binary_add);
    ck_assert_msg(a2 == 26, "accumulate() returned unexpected result for binary add");

    const int a3 = accumulate(array, 5, 4, 0, binary_add);
    ck_assert_msg(a3 == 0, "accumulate() returned incorrect result when provided invalid indices");
}
END_TEST

START_TEST(test_accumulate_mul)
{
    int array[N_ITEMS];
    initialize_array(array, N_ITEMS);

    const int a1 = accumulate(array, 0, N_ITEMS, 1, binary_mul);
    ck_assert_msg(a1 == 40320, "accumulate() returned unexpected result for binary mul");

    const int a2 = accumulate(array, 0, N_ITEMS, 0, binary_mul);
    ck_assert_msg(a2 == 0, "accumulate() returned unexpected result for binary mul");

    const int a3 = accumulate(array, 5, 4, 1, binary_mul);
    ck_assert_msg(a3 == 0, "accumulate() returned incorrect result when provided invalid indices");
}
END_TEST

// ----------------------------------------------------------------------------
// Infrastructure
 
Suite* accumulate_suite(void)
{
    Suite* s = suite_create("accumulate");
    TCase* tc_core = tcase_create("accumulate-core");
    
    tcase_add_test(tc_core, test_accumulate_add);
    tcase_add_test(tc_core, test_accumulate_mul);
    
    suite_add_tcase(s, tc_core);
    
    return s;
}

int main(void)
{
    Suite* suite = accumulate_suite();
    SRunner* runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);    
    srunner_free(runner);
    
    return EXIT_SUCCESS;
}
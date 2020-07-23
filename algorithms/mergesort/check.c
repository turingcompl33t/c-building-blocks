// check.c
// Driver program for mergesort algorithm tests.

// attribute gnu_printf
#pragma GCC diagnostic ignored "-Wignored-attributes"

#include <time.h>
#include <check.h>
#include <stdlib.h>

#include "mergesort.h"

#define N_ITEMS 100

// ----------------------------------------------------------------------------
// Definitions for Testing

static bool less_equal(int a, int b)
{
    return a <= b;
}

static bool greater_equal(int a, int b)
{
    return a >= b;
}

static void random_array(
    int array[], 
    size_t len, 
    size_t lower, 
    size_t upper)
{
    for (size_t i = 0; i < len; ++i)
    {
        int n = (rand() % (upper - lower + 1)) + lower;
        array[i] = n;
    }
}

static bool is_sorted(
    int array[], 
    size_t begin, 
    size_t end, 
    comparator_f cmp)
{
    for (size_t i = begin; i < end; ++i)
    {
        if (!cmp(array[i], array[i+1]))
        {
            return false;
        }
    }

    return true;
}

// ----------------------------------------------------------------------------
// Test Cases

START_TEST(test_mergesort)
{
    int src[N_ITEMS];
    int dst[N_ITEMS];

    // test ascending sorts
    random_array(src, N_ITEMS, 0, 1000);
    random_array(dst, N_ITEMS, 0, 1000);

    mergesort(src, dst, 0, N_ITEMS - 1, less_equal);
    ck_assert_msg(is_sorted(dst, 0, N_ITEMS - 1, less_equal));

    // test descending sorts
    random_array(src, N_ITEMS, 0, 1000);
    random_array(dst, N_ITEMS, 0, 1000);

    mergesort(src, dst, 0, N_ITEMS - 1, greater_equal);
    ck_assert_msg(is_sorted(dst, 0, N_ITEMS - 1, greater_equal));
}
END_TEST

// ----------------------------------------------------------------------------
// Infrastructure
 
Suite* mergesort_suite(void)
{
    Suite* s = suite_create("mergesort");
    TCase* tc_core = tcase_create("mergesort-core");
    
    tcase_add_test(tc_core, test_mergesort);
    
    suite_add_tcase(s, tc_core);
    
    return s;
}

int main(void)
{
    srand(1);

    Suite* suite = mergesort_suite();
    SRunner* runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);    
    srunner_free(runner);
    
    return EXIT_SUCCESS;
}
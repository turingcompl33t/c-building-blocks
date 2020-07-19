// check.c
// Driver program for bubble sort algorithm tests.

#include <time.h>
#include <check.h>
#include <stdlib.h>

#include "bubble_sort.h"

#define N_ITEMS 100

// ----------------------------------------------------------------------------
// Definitions for Testing

static bool less(int a, int b)
{
    return a < b;
}

static bool greater(int a, int b)
{
    return a > b;
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
    policy_f policy)
{
    for (size_t i = begin; i < end; ++i)
    {
        if (policy(array[i], array[i+1]))
        {
            return false;
        }
    }

    return true;
}

// ----------------------------------------------------------------------------
// Test Cases

START_TEST(test_bubble_sort)
{
    int arr1[N_ITEMS];
    int arr2[N_ITEMS];

    random_array(arr1, N_ITEMS, 0, 1000);
    random_array(arr2, N_ITEMS, 0, 1000);

    bubble_sort(arr1, 0, N_ITEMS, less);     // descending sort
    ck_assert_msg(is_sorted(arr1, 0, N_ITEMS, less), "bubble_sort() failed to produce sorted array");

    bubble_sort(arr2, 0, N_ITEMS, greater);  // ascending sort
    ck_assert_msg(is_sorted(arr2, 0, N_ITEMS, greater), "bubble_sort() failed to produce sorted array");
}
END_TEST

// ----------------------------------------------------------------------------
// Infrastructure
 
Suite* bubble_sort_suite(void)
{
    Suite* s = suite_create("bubble-sort");
    TCase* tc_core = tcase_create("bubble-sort-core");
    
    tcase_add_test(tc_core, test_bubble_sort);
    
    suite_add_tcase(s, tc_core);
    
    return s;
}

int main(void)
{
    srand(1);

    Suite* suite = bubble_sort_suite();
    SRunner* runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);    
    srunner_free(runner);
    
    return EXIT_SUCCESS;
}
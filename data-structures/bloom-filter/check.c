// check.c
// Driver program for stack data structure tests.

// attribute gnu_printf
#pragma GCC diagnostic ignored "-Wignored-attributes"

#include <check.h>
#include <stdlib.h>

#include "bloom_filter.h"

// ----------------------------------------------------------------------------
// Test Cases

START_TEST(test_bloom_filter)
{
    bloom_filter_t* filter = filter_new(64, 3);
    ck_assert_msg(filter != NULL, "filter_new() returned NULL");

	filter_insert(filter, (byte_t*)"abc", 3); 
	filter_insert(filter, (byte_t*)"def", 3);
	filter_insert(filter, (byte_t*)"ghi", 3);

    const filter_test_t r1 = filter_test(filter, (byte_t*)"abc", 3);
    const filter_test_t r2 = filter_test(filter, (byte_t*)"def", 3);
    const filter_test_t r3 = filter_test(filter, (byte_t*)"ghi", 3);

    ck_assert_msg(r1 == PRESENT, "filter_test() returned incorrect result");
    ck_assert_msg(r2 == PRESENT, "filter_test() returned incorrect result");
    ck_assert_msg(r3 == PRESENT, "filter_test() returned incorrect result");

	filter_clear(filter);

    const filter_test_t r4 = filter_test(filter, (byte_t*)"abc", 3);
    const filter_test_t r5 = filter_test(filter, (byte_t*)"def", 3);
    const filter_test_t r6 = filter_test(filter, (byte_t*)"ghi", 3);

    ck_assert_msg(r4 == ABSENT, "filter_test() returned incorrect result");
    ck_assert_msg(r5 == ABSENT, "filter_test() returned incorrect result");
    ck_assert_msg(r6 == ABSENT, "filter_test() returned incorrect result");

	filter_stats_t stats = filter_stats(filter); 

    ck_assert_msg(stats.n_items == 0, "filter_stats() returned incorrect value");
    ck_assert_msg(stats.n_bits == 64, "filter_stats() returned incorrect value");
    ck_assert_msg(stats.n_setbits == 0, "filter_stats() returned incorrect value");

    filter_delete(filter);
}
END_TEST

// ----------------------------------------------------------------------------
// Infrastructure
 
Suite* bloom_filter_suite(void)
{
    Suite* s = suite_create("bloom-filter");
    TCase* tc_core = tcase_create("bloom-filter-core");
    
    tcase_add_test(tc_core, test_bloom_filter);
    
    suite_add_tcase(s, tc_core);
    
    return s;
}

int main(void)
{
    Suite* suite = bloom_filter_suite();
    SRunner* runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);    
    srunner_free(runner);
    
    return EXIT_SUCCESS;
}
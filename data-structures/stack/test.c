// test.c
// Driver program for stack data structure tests.

#include <check.h>
#include <stdlib.h>

#include "stack.h"

START_TEST(test_stack_new)
{
    // new stack construction should succeed
    stack_t* s = stack_new();
    ck_assert(s != NULL);

    // new stack should be empty
    const size_t count = stack_count(s);
    ck_assert(count == 0);

    // destruction of stack should succeed because it is empty
    const bool success = stack_delete(s);
    ck_assert(success);
}
END_TEST
 
Suite* stack_suite(void)
{
    Suite* s = suite_create("stack");
    TCase* tc_core = tcase_create("stack-core");
    
    tcase_add_test(tc_core, test_stack_new);
    
    suite_add_tcase(s, tc_core);
    
    return s;
}

int main(void)
{
    Suite* suite = stack_suite();
    SRunner* runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);
    
    const int n_failed = srunner_ntests_failed(runner);
    
    srunner_free(runner);
    
    return (n_failed == 0) 
        ? EXIT_SUCCESS 
        : EXIT_FAILURE;
}
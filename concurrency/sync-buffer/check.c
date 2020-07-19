// check.c
// Driver program for stack data structure tests.

#include <check.h>
#include <stdlib.h>
#include <pthread.h>

#include "sync_buffer.h"

#define CONCURRENCY_LEVEL 5
#define OPS_PER_THREAD    100

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

static void* producer(void* arg)
{
    sync_buffer_t* buffer = (sync_buffer_t*) arg;

    size_t put_count = 0;
    for (size_t i = 0; i < OPS_PER_THREAD; ++i)
    {
        point_t* p = make_point(3, 4);
        sync_buffer_put(buffer, p);
        put_count++;
    }

    return (void*) put_count;
}

static void* consumer(void* arg)
{
    sync_buffer_t* buffer = (sync_buffer_t*) arg;

    size_t get_count = 0;
    for (size_t i = 0; i < OPS_PER_THREAD; ++i)
    {
        point_t* p = sync_buffer_get(buffer);
        delete_point(p);
        get_count++;
    }

    return (void*) get_count;
}

// ----------------------------------------------------------------------------
// Test Cases

START_TEST(test_sync_buffer)
{
    pthread_t producers[CONCURRENCY_LEVEL];
    pthread_t consumers[CONCURRENCY_LEVEL];

    sync_buffer_t* buffer = sync_buffer_new(32);
    ck_assert_msg(buffer != NULL, "sync_buffer_new() returned NULL");

    // create n producer and consumer threads
    for (size_t i = 0; i < CONCURRENCY_LEVEL; ++i)
    {
        const int pret = pthread_create(&producers[i], NULL, producer, buffer);
        const int cret = pthread_create(&consumers[i], NULL, consumer, buffer);

        ck_assert(0 == pret);
        ck_assert(0 == cret);
    }

    void* producer_ret;
    void* consumer_ret;

    size_t produce_count = 0;
    size_t consume_count = 0;

    // join with all of our threads
    for (size_t i = 0; i < CONCURRENCY_LEVEL; ++i)
    {
        pthread_join(producers[i], &producer_ret);
        pthread_join(consumers[i], &consumer_ret);

        produce_count += (size_t)producer_ret;
        consume_count += (size_t)consumer_ret;
    }

    const size_t expected = CONCURRENCY_LEVEL*OPS_PER_THREAD;

    ck_assert_msg(produce_count == expected, "produce count differs from expected");
    ck_assert_msg(consume_count == expected, "consume count differs from expected");

    const bool destroyed = sync_buffer_delete(buffer);
    ck_assert_msg(destroyed, "sync_buffer_destroy() failed for expected empty buffer");
}
END_TEST

// ----------------------------------------------------------------------------
// Infrastructure
 
Suite* sync_buffer_suite(void)
{
    Suite* s = suite_create("sync-buffer");
    TCase* tc_core = tcase_create("sync-buffer-core");
    
    tcase_add_test(tc_core, test_sync_buffer);

    suite_add_tcase(s, tc_core);
    
    return s;
}

int main(void)
{
    Suite* suite = sync_buffer_suite();
    SRunner* runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);    
    srunner_free(runner);
    
    return EXIT_SUCCESS;
}
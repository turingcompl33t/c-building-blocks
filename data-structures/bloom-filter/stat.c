// stat.c
// A simple statistical test driver for bloom filter.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "bloom_filter.h"

// ----------------------------------------------------------------------------
// Test Control Constants 

#define ITEM_SIZE            8       // the size of each inserted item, in bytes

#define N_ITEMS_INSERT       8192    // number of elements to insert
#define N_ITEMS_TEST         32768   // number of elements to test for presence 

#define MIN_FILTER_SIZE      512     // minimum number of bits in filter 
#define MAX_FILTER_SIZE      32768   // maximum number of bits in filter 
#define DEFAULT_FILTER_SIZE  16384   // for hash test 

#define MIN_HASHES            2      // minimum number of hashes to use 
#define MAX_HASHES            5      // maximum number of hashes to use 
#define DEFAULT_HASHES        3      // for filter size test 

// ----------------------------------------------------------------------------
// Internal Declarations

typedef struct monotonic_byte_seq
{
	size_t count;
} monotonic_byte_seq_t;

void test_case(size_t n_bits, size_t n_hashes);
void get_n_bytes(byte_t buffer[], size_t n_bytes, byte_t value);

// ----------------------------------------------------------------------------
// Main

int main(void) 
{
	// test filter sizes (increment by powers of 2) 
	for (size_t fs = MIN_FILTER_SIZE; fs <= MAX_FILTER_SIZE; fs*=2) 
	{
		test_case(fs, DEFAULT_HASHES); 
	}

	// test number of hashes used 
	for (size_t nh = MIN_HASHES; nh <= MAX_HASHES; ++nh) 
	{
		test_case(DEFAULT_FILTER_SIZE, nh); 
	}

	return EXIT_SUCCESS;
}

// ----------------------------------------------------------------------------
// Test Driver

void test_case(size_t n_bits, size_t n_hashes)
{
	bloom_filter_t* filter = filter_create(n_bits, n_hashes); 
	if (NULL == filter) 
	{
		printf("ERROR: failed to initialize filter\n");
		exit(1);
	}	

	// insert N_ELEMENTS keys into filter 
	byte_t buffer[ITEM_SIZE];
	for (size_t i = 0; i < N_ITEMS_INSERT; ++i) 
	{
		get_n_bytes(buffer, ITEM_SIZE, i);
		filter_insert(filter, buffer, ITEM_SIZE); 
	}

	// how did we do? 
	size_t tru_pos = 0;
	size_t fal_pos = 0;
	size_t tru_neg = 0;
	size_t fal_neg = 0;
	size_t n_error = 0;

	for (size_t i = 0; i < N_ITEMS_TEST; ++i) 
	{
		get_n_bytes(buffer, ITEM_SIZE, i);
		const filter_test_t ret = filter_test(filter, i);

		if (PRESENT == ret) 
		{
			// report key present 
			if (i < N_ITEMS_INSERT) 
			{
				// is actually present -> true positive
				++tru_pos; 
			} 
			else 
			{
				// is not present -> false positive 
				++fal_pos;
			}
		} 
		else if (ABSENT == ret) 
		{
			// report key not present
			if (i < N_ITEMS_INSERT) 
			{
				// is actually present -> false negative
				++fal_neg;
			} 
			else 
			{
				// is not present -> true negative
				++tru_neg;
			}
		} 
		else 
		{
			// got an error
			++n_error;
		}
	}

	printf("-------------------------------------------------------------------\n");
	printf("TEST CASE: filter size = %d, n hashes = %d\n", filter->n_bits, filter->n_hashes);
	printf("elements inserted = %d, elements tested = %d\n", N_ITEMS_INSERT, N_ITEMS_TEST);
	printf("true positives   = %d\n", tru_pos);
	printf("true negatives   = %d\n", tru_neg);
	printf("false positives  = %d\n", fal_pos);
	printf("false negatives  = %d\n", fal_neg); 
	printf("-------------------------------------------------------------------\n\n");

	// cleanup
	filter_destroy(filter); 
}

// ----------------------------------------------------------------------------
// Internal

void get_n_bytes(byte_t buffer[], size_t n_bytes, byte_t value)
{
	for (size_t i = 0; i < n_bytes; ++i)
	{
		buffer[i] = value;
	}
}

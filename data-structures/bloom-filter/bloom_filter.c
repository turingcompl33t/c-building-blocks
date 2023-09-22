// bloom_filter.c
// A probabilistic set implemented as a bloom filter.

#include <math.h>
#include <stdlib.h>
#include <limits.h>

#include "murmur3.h"
#include "bloom_filter.h"

// set bit k in bit vector A
#define SET_BIT(A,k) ( A[(k/8)] |=  (1 << (k % 8)) )

// test bit k in bit vector A
#define TEST_BIT(A,k) ( A[(k/8)] &   (1 << (k % 8)) )

// clear bit k in bit vector A 
#define CLEAR_BIT(A,k) ( A[(k/8)] &= ~(1 << (k % 8)) )

bloom_filter_t* filter_new(const size_t n_bits, const size_t n_hashes)
{
	if (0 == n_bits || n_hashes == 0)
	{
		// invalid parameters
		return NULL;
	}

	bloom_filter_t* filter = malloc(sizeof(bloom_filter_t));
	if (NULL == filter)
	{
		return NULL;
	}

	// compute the minimum number of bytes needed to represent
	// a vector of the desired length
	const size_t vector_len 
		= (size_t) ceil((float) n_bits / (float) (sizeof(uint8_t)*CHAR_BIT)); 

	uint8_t* vector = calloc(vector_len, sizeof(uint8_t));
	if (NULL == vector)
	{
		free(filter);
		return NULL;
	}

	filter->bitvector  = vector;
	filter->n_items    = 0;
	filter->n_hashes   = n_hashes;
	filter->n_bits     = n_bits;
	filter->n_setbits  = 0;

	return filter;
}

void filter_delete(bloom_filter_t* filter)
{
	if (filter != NULL)
	{
		if (filter->bitvector != NULL)
		{
			free(filter->bitvector);
		}

		free(filter);
	}
}

void filter_insert(bloom_filter_t* filter, byte_t* data, size_t len)
{
	if (NULL == filter || NULL == data || 0 == len)
	{
		return;
	}

	uint32_t hash;
	uint32_t seed = 0;

	for (size_t i = 0; i < filter->n_hashes; ++i) 
	{
		// perform the hash
		MurmurHash3_x86_32(data, len, seed++, &hash); 

		// determine bit to set based on filter size 
		const size_t bit = hash % filter->n_bits;

		// made insertion slightly more complex than necessary 
		// in order to track number of bits set in filter
		if (!TEST_BIT(filter->bitvector, bit)) 
		{
			// set the bit in the filter
			SET_BIT(filter->bitvector, bit); 
			filter->n_setbits++; 
		}
	}

	filter->n_items++; 
}

filter_test_t filter_test(bloom_filter_t* filter, byte_t* data, size_t len)
{
	if (NULL == filter || NULL == data || 0 == len)
	{
		return ERROR;
	}

	uint32_t hash;
	uint32_t seed = 0;

	for (size_t i = 0; i < filter->n_hashes; ++i) 
	{
		// do the hash 
		MurmurHash3_x86_32(data, len, seed, &hash);

		// determine bit to set based on filter size 
		const size_t bit = hash % filter->n_bits; 

		// test if bit is set if not, can 
		// immediately report key not in filter 
		if (!TEST_BIT(filter->bitvector, bit)) 
		{
			return ABSENT; 
		}
	}

	// all bits corresponding to key set, report key in filter 
	return PRESENT;
}

void filter_clear(bloom_filter_t* filter)
{
	if (NULL == filter)
	{
		return;
	}

	// clear all of the bits in internal bit vector 
	for (size_t i = 0; i < filter->n_bits; ++i) 
	{
		CLEAR_BIT(filter->bitvector, i);
	}

	// reset running counts 
	filter->n_items    = 0;
	filter->n_setbits  = 0;
}

filter_stats_t filter_stats(bloom_filter_t* filter)
{
	filter_stats_t stats = {
		.n_items   = 0,
		.n_bits    = 0,
		.n_setbits = 0
	};

	if (filter != NULL)
	{
		stats.n_items   = filter->n_items;
		stats.n_bits    = filter->n_bits;
		stats.n_setbits = filter->n_setbits;
	}

	return stats;
}
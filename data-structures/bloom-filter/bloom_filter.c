// bloom_filter.c
// A probabilistic set implemented as a bloom filter.

#include <stdlib.h>

#include "murmur3.h"
#include "bloom_filter.h"

bloom_filter_t* filter_new(const size_t n_bits, const size_t n_hashes)
{
	return NULL;
}

void filter_delete(bloom_filter_t* filter)
{
	return;
}

void filter_insert(bloom_filter_t* filter, byte_t* data, size_t len)
{
	return;
}

filter_test_t filter_test(bloom_filter_t* filter, byte_t* data, size_t len)
{
	return ERROR;
}

void filter_clear(bloom_filter_t* filter)
{
	return;
}

filter_stats_t filter_stats(bloom_filter_t* filter)
{
	filter_stats_t stats;
	stats.n_items   = 0;
	stats.n_bits    = 0;
	stats.n_setbits = 0;

	return stats;
}
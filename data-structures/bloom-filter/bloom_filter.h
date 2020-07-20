// bloom_filter.h
// A probabilistic set implemented as a bloom filter.

#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Background:
//
// From the excellent "Bloom Filters By Example" article:
// (https://llimllib.github.io/bloomfilter-tutorial/)
//
// "A bloom filter is a data structure designed to tell you,
// rapidly and memory-efficiently, whether an element is present
// in a set.
//
// The price paid for this efficiency is that a Bloom filter
// is a probabilistic data structure: it tells us that the 
// element either _definitely is not_ in the set or _may be_ 
// in the set."
//
// The way that a bloom filter accomplishes its efficiency
// is by not stored the inserted set elements themselves,
// but rather computing hashes of the inserted elements and
// using these hashes to set and test bits in an internal bit
// vector. 
//
// An insertion of an item into a bloom filter proceeds (roughly) as follows:
//
// For each hash function utilized by the filter:
//	Hash the inserted item
//	Determine the bit to set in the filter based on this hash 
//	Set this bit in the filter
//
// The test for presence of an item into a bloom filter proceeds (roughly) as follows:
//
// For each hash function utilized by the filter:
// 	Hash the inserted item
//	Determine the bit to test in the filter based on this hash
//	Test this bit
//		If the bit is set, continue to the next has
//		If the bit is not set, the item is not in the filter, return the item is not present
// All bits have been tested, return the item is present 
// 	
// The probabilistic nature of the Bloom filter enters the picture
// because the limited size of the bit vector implies that we will
// have collisions - instances in which two distinct items produce a
// hash that encodes the same bit in the internal bit vector. One way
// in which Bloom filter implementations decrease the probability of
// these collisions is by increasing the number of hash functions used
// by the filter - the greater the number of hash functions, the lower
// the probability of receiving a false poisitive result from a test.
//
// This implementation uses the Murmur3 hash function to perform the
// hashing within the filter. The Murmur3 library is already included
// (as source) in this directory and will be linked to the driver
// program so that your filter implementation can make use of it. See the
// header file (murmur3.h) for more details on its use.

// The representation of bytes-like objects.
typedef uint8_t byte_t;

// The type returned by filter_test().
typedef enum 
{
	ABSENT,
	PRESENT,
	ERROR
} filter_test_t; 

// The type returned by filter_stats().
typedef struct filter_stats
{
	// The number of items inserted into the filter. 
	size_t n_items;

	// The number of bits in the filter.
	size_t n_bits;

	// The number of set bits in the filter.
	size_t n_setbits;
} filter_stats_t;

// The bloom filter data structure.
typedef struct bloom_filter {
	// The number of bits used in filter.
	size_t n_bits;
	
	// The number of hash functions used to implement filter. 
	size_t n_hashes;

	// The number of items inserted into filter.
	size_t n_items;

	// The number of bits set in the filter
	size_t n_setbits;

	// The bloom filter bit vector
	uint8_t* bitvector;
} bloom_filter_t;

// filter_new()
//
// Construct a new bloom filter data structure.
//
// Arguments:
//	n_bits   - the number of bits in internal filter bit vector
//  n_hashes - the number of hash functions to use in filter operations
//
// Returns:
//	A pointer to a newly constructed bloom filter on success
//	NULL on failure (invalid arguments, allocation failure)
bloom_filter_t* filter_new(const size_t n_bits, const size_t n_hashes);

// filter_delete()
//
// Destroy an existing bloom filter.
//
// Arguments:
//	filter - pointer to existing bloom filter
void filter_delete(bloom_filter_t* filter);

// bloom_insert()
//
// Insert new data into the bloom filter.
//
// Arguments:
//	filter - pointer to an existing bloom filter
//	data   - arbitrary user data to insert
//  len    - the length of the user data (in bytes)
void filter_insert(bloom_filter_t* filter, byte_t* data, size_t len);

// filter_test()
//
// Test for the presence of `data` in existing bloom filter.
//
// Arguments:
//	filter - pointer to existing bloom filter
//	data   - arbitrary user data for which to test
//	len    - the length of the user data (in bytes)
//
// Returns:
//	ABSENT  if the data is not present in the filter
//	PRESENT if the data is likely present in the filter
//	ERROR   on error 
filter_test_t filter_test(bloom_filter_t* filter, byte_t* data, size_t len);

// filter_clear()
//
// Clear all data from the filter.
//
// Arguments:
//	filter - pointer to existing bloom filter
void filter_clear(bloom_filter_t* filter);

// filter_stats()
//
// Return metadata about the current state of the filter.
//
// Arguments:
//	filter - pointer to existing bloom filter
//
// Returns:
//	A structure populated with metadata regarding the filter state
filter_stats_t filter_stats(bloom_filter_t* filter); 

#endif // BLOOM_FILTER_H 
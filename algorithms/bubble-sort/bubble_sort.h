// bubble_sort.h
// Bubble sort sorting algorithm.

#ifndef BUBBLE_SORT_H
#define BUBBLE_SORT_H

#include <stddef.h>
#include <stdbool.h>

// Background:
//
// Bubble sort is a very simple sorting algorithm famous
// both for the simplicity of its implementation as well
// as its inefficiency.
//
// The general algorithm for implementing bubble sort is:
//
// For each element in the array:
//  Determine its proper location in the final sorted
//  array by comparing it with each other element in the array
//
// As an example, suppose we have an array wtih the following elements:
//
// [4, 2, 3, 1]
//
// and we want to sort this array into ascending order. The stages
// the array would take during the course of the bubble sort algorithm
// would be the following:
//
// [4, 2, 3, 1] <- start first pass
// [4, 2, 3, 1] <- compare 4 and 2
// [2, 4, 3, 1] <- compare 4 and 3
// [2, 3, 4, 1] <- compare 4 and 1
// [2, 3, 1, 4] <- end first pass / start second pass
// [2, 3, 1, 4] <- compare 2 and 3
// [2, 3, 1, 4] <- compare 3 and 1
// [2, 1, 3, 4] <- compare 3 and 4
// [2, 1, 3, 4] <- end second pass / start third pass
// [2, 1, 3, 4] <- compare 2 and 1
// [1, 2, 3, 4] <- compare 2 and 3
// [1, 2, 3, 4] <- compare 3 and 4
// [1, 2, 3, 4] <- end third pass / start fourth pass
// [1, 2, 3, 4] <- compare 1 and 2
// [1, 2, 3, 4] <- compare 2 and 3
// [1, 2, 3, 4] <- compare 3 and 4
// [1, 2, 3, 4] <- complete
//
// In general, sorting algorithms may be implemented as
// in-place sorts wherein the array to be sorted is modified
// directly to produce a sorted array, or external sorts
// where a new sorted array is produced, independent of the
// original input array. Bubble sort is simple enough to
// implement as an in-place sort, so that is the approach
// taken in this module.

// The signature for the user-provided sort policy function.
//
// The semantics for this function are:
// This function returns `true` if argument
// `a` is ordered after argument `b` in the 
// final desired sort order.
typedef bool (*policy_f)(int a, int b);

// bubble_sort()
//
// Sort the array `array` from index `begin`
// up through but excluding index `end` according
// to the policy function `should_swap`.
//
// After this function completes, the elements
// of array `array` in the range [begin, end - 1]
// are in sorted order according to provided policy.
//
// Arguments:
//  array       - the array of integers to sort
//  begin       - the index at which sort begins
//  end         - the index at which sort ends
//  should_swap - the policy function that determines sort order
void bubble_sort(
    int array[], 
    size_t begin, 
    size_t end, 
    policy_f should_swap);

#endif // BUBBLE_SORT_H
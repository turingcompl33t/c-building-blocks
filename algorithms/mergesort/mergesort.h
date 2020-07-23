// mergesort.h
// The mergesort comparison sort algorithm.

#ifndef MERGESORT_H
#define MERGESORT_H

#include <stddef.h>
#include <stdbool.h>

// The user-provided comparison function.
//
// The semantics of this function are:
//  - Returns `true` if `a` should be ordered
//    before `b` in sorted order
//  - Returns `false` otherwise
typedef bool (*comparator_f)(int a, int b);

// mergesort()
//
// Sort the array `src` between index `lo` and 
// index `hi` (inclusive) using the mergesort
// comparison sort algorithm, according to the 
// provided binary comparison function `cmp`, 
// resulting in a sorted array in destination `dst`.
//
// Arguments:
//  src - the array to be sorted
//  dst - the output sorted array
//  lo  - the low index of the array where sort should begin
//  hi  - the high index of the array where sort should end
//  cmp - the user-provided comparator
void mergesort(int src[], int dst[], size_t lo, size_t hi, comparator_f cmp);

#endif // MERGESORT_H
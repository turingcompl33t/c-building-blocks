// quicksort.h
// The quicksort comparison sort algorithm.

#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <stddef.h>
#include <stdbool.h>

// The user-provided comparison function.
//
// The semantics of this function are:
//  - Returns `true` if `a` should be ordered
//    before `b` in sorted order
//  - Returns `false` otherwise
typedef bool (*comparator_f)(int a, int b);

// quicksort()
//
// Sort the array `array` between index `lo` and 
// index `hi` (inclusive) using the quicksort
// comparison sort algorithm, according to the 
// provided binary comparison function `cmp`.
//
// Arguments:
//  array - the array to be sorted
//  lo    - the low index of the array where sort should begin
//  hi    - the high index of the array where sort should end
//  cmp   - the user-provided comparator
void quicksort(int array[], size_t lo, size_t hi, comparator_f cmp);

#endif // QUICKSORT_H
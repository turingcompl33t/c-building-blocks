// accumulate.h
// Accumulate algotithm on integer arrays.

#ifndef ACCUMULATE_H
#define ACCUMULATE_H

#include <stddef.h>

// Background:
//
// The accumulate algorithm is a special case of a reduction algorithm
// in which multiple values stored in some container are accumulated
// into a single value according to some function and parameters
// supplied as arguments to the algorithm. The algorithm is best illustrated
// via an example. Consider an array with the following contents:
//
// [1, 2, 3]
//
// Further, suppose that the binary operation supplied to the algorithm
// is binary addition and that the initial value supplied to the algorithm
// is 1. If we wanted to accumulate over the entire array, the algorithm 
// would proceed as follows:
//
// 0. Accumulator = 1 (starting value)
// 1. Accumulator = Accumulator + 1 => 1
// 2. Accumulator = Accumulator + 2 => 3
// 3. Accumulator = Accumulator + 3 => 6
//
// Thus, the result of accumulating the elements of this array
// with the binary operation add(x, y) and the initial value 1
// is 6.
//
// One of the beautiful aspects of algorithms like accumulate, however,
// is that their behavior may be altered significantly based on the
// parameters they are provided. For instance, in the example above,
// selecting add(x, y) as the binary operation produced an algorithm that
// computed the sum of the elements in the array. However, one might imagine 
// providing mul(x, y) as the binary operation to accumulate 
// (instead of add(x, y)) in order to compute the product of the elements in an array.

// The signature of the binary operation
// provided to the accumulation algorithm. 
typedef int (*binary_op_f)(int a, int b);

// accumulate()
//
// Accumulates the values provided in `array` 
// with the starting value `start`, beginning 
// at index `first` and continuing through the
// index preceding `last`, via the binary 
// operator `op`.
//
// Arguments:
//  array - the array of values to accumulate
//  first - the index at which to begin accumulation
//  last  - the index at which to end accumulation
//  start - the value at which to initialize the accumulator
//  op    - the binary operation to apply during accumulation
//
// Returns:
//  The accumulation produced according the algorithm described above 
int accumulate(
    int array[], 
    size_t first, 
    size_t last, 
    int start, 
    binary_op_f op);

#endif // ACCUMULATE_H
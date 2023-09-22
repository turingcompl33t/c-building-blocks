// accumulate.c
// Accumulate algotithm on integer arrays.

#include "accumulate.h"

int accumulate(
    int array[], 
    size_t first, 
    size_t last, 
    int start, 
    binary_op_f op)
{
    if (first >= last)
    {
        return 0;
    }

    int accumulator = start;
    for (size_t i = first; i < last; ++i)
    {
        accumulator = op(accumulator, array[i]);
    }

    return accumulator;
}
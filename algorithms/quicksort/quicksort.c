// quicksort.c
// The quicksort comparison sort algorithm.

#include <stdio.h>
#include "quicksort.h"

static size_t partition(int array[], size_t lo, size_t hi, comparator_f cmp);
static void swap(int array[], size_t i, size_t j);

void quicksort(int array[], size_t lo, size_t hi, comparator_f cmp)
{
    if (lo < hi)
    {
        size_t p = partition(array, lo, hi, cmp);
        quicksort(array, lo, p - 1, cmp);
        quicksort(array, p + 1, hi, cmp);
    }
}

static size_t partition(int array[], size_t lo, size_t hi, comparator_f cmp)
{
    int pivot = array[hi];
    size_t i = lo;
    for (size_t j = lo; j < hi; ++j)
    {
        if (array[j] < pivot)
        {
            swap(array, i, j);
            ++i;
        }
    }

    swap(array, i, hi);

    return i;
}

static void swap(int array[], size_t i, size_t j)
{
    int const tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}
// mergesort.c

#include <string.h>

#include "mergesort.h"

// ----------------------------------------------------------------------------
// Internal Declarations

static void split_and_merge(
    int dst[], 
    int src[], 
    size_t lo, 
    size_t hi, 
    comparator_f cmp);

static void merge(
    int dst[], 
    int src[], 
    size_t lo, 
    size_t mid, 
    size_t hi, 
    comparator_f cmp);

// ----------------------------------------------------------------------------
// Exported

void mergesort(int src[], int dst[], size_t lo, size_t hi, comparator_f cmp)
{
    memcpy(dst, src, sizeof(int)*(hi - lo));
    split_and_merge(dst, src, lo, hi + 1, cmp);
}

// ----------------------------------------------------------------------------
// Internal

static void split_and_merge(
    int dst[], 
    int src[], 
    size_t lo, 
    size_t hi, 
    comparator_f cmp)
{
    if (hi - lo <= 1)
    {
        return;
    }

    const size_t mid = (hi + lo) / 2;
    
    split_and_merge(src, dst, lo, mid, cmp);
    split_and_merge(src, dst, mid, hi, cmp);

    merge(dst, src, lo, mid, hi, cmp);
}

static void merge(
    int dst[], 
    int src[], 
    size_t lo, 
    size_t mid, 
    size_t hi, 
    comparator_f cmp)
{
    size_t i = lo;
    size_t j = mid;

    for (size_t k = lo; k < hi; ++k)
    {
        if (i < mid && (j >= hi || cmp(src[i], src[j])))
        {
            dst[k] = src[i];
            ++i;
        }
        else
        {
            dst[k] = src[j];
            ++j;
        }
    }
}
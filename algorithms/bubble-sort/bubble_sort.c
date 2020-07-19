// bubble_sort.c
// Bubble sort sorting algorithm.

#include "bubble_sort.h"

// ----------------------------------------------------------------------------
// Internal Declarations

static void swap(int array[], size_t index_a, size_t index_b);

// ----------------------------------------------------------------------------
// Exported

void bubble_sort(
    int array[], 
    size_t begin, 
    size_t end, 
    policy_f should_swap)
{
    if (begin >= end)
    {
        return;
    }

    bool iteration_did_swap;

    do
    {
        iteration_did_swap = false;

        for (size_t i = begin; i < end; ++i)
        {
            if (should_swap(array[i], array[i+1]))
            {
                swap(array, i, i+1);
                iteration_did_swap = true;
            }
        }

    } while (iteration_did_swap);
}

// ----------------------------------------------------------------------------
// Internal

static void swap(int array[], size_t index_a, size_t index_b)
{
    int tmp = array[index_a];
    array[index_a] = array[index_b];
    array[index_b] = tmp;
}
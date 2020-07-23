// driver.c
// Driver program for Conway's Game of Life simulator.
// 
// Constructs a glider and runs the simulation indefinitely.

#include "conway.h"

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>

// arbitrarily set minimum number of rows and columns to 5
#define MIN_N_ROWS 5
#define MIN_N_COLS 5

// ----------------------------------------------------------------------------
// Internal Declarations

typedef struct args
{
    bool   success;
    size_t n_rows;
    size_t n_cols;
} args_t;

static args_t parse_args(int argc, char* argv[]);

// ----------------------------------------------------------------------------
// Driver

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("[-] Error: invalid arguments");
        printf("[-] Usage: %s <ROWS> <COLUMNS>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const args_t args = parse_args(argc, argv);
    if (!args.success)
    {
        printf("[-] Error: invalid arguments\n");
        return EXIT_FAILURE;
    }

    // create a new context
    conway_ctx_t* ctx = conway_new(args.n_rows, args.n_cols);

    // set the cells to construct a glider
    conway_set_cell(ctx, 2, 0);
    conway_set_cell(ctx, 2, 1);
    conway_set_cell(ctx, 2, 2);
    conway_set_cell(ctx, 1, 2);
    conway_set_cell(ctx, 0, 1);

    // print the initial state of the grid
    conway_print_grid(ctx);

    // run the simulation
    for (;;)
    {
        conway_simulate_n(ctx, 1);
        conway_print_grid(ctx);
        
        sleep(1);
    }

    // cleanup the context
    conway_delete(ctx);

    return EXIT_SUCCESS;
}

// ----------------------------------------------------------------------------
// Internal

static args_t parse_args(int argc, char* argv[])
{
    const size_t n_rows = strtoull(argv[1], NULL, 0);
    const size_t n_cols = strtoull(argv[2], NULL, 0);

    const bool success = (errno != ERANGE) 
        && (n_rows >= MIN_N_ROWS) 
        && (n_cols >= MIN_N_COLS); 

    args_t args = {
        .success = success,
        .n_rows  = success ? n_rows : 0,
        .n_cols  = success ? n_cols : 0
    };

    return args;
}
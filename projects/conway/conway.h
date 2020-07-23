// conway.h
// Implementation of John Conway's Game of Life cellular automaton.

#ifndef CONWAY_H
#define CONWAY_H

#include <stddef.h>

// The top-level ccontext for the simulation.
// What data might need to be persisted here?
typedef struct conway_ctx conway_ctx_t;

// conway_new()
//
// Construct a new context for a Game of Life simulation.
conway_ctx_t* conway_new(size_t n_rows, size_t n_cols);

// conway_delete()
//
// Destroy an existing Game of Life context.
//
// Arguments:
// ctx - pointer to a context structure created by 
//       previous call to conway_new()
void conway_delete(conway_ctx_t* ctx);

// conway_set_cell()
//
// Set a the cell at the specified location to the `live`
// state; intended to be used for initial context setup.
//
// Arguments:
// ctx - pointer to a context structure created by 
//       previous call to conway_new()
void conway_set_cell(conway_ctx_t* ctx, size_t row_idx, size_t col_idx);

// conway_unset_cell()
//
// Set a the cell at the specified location to the `dead`
// state; intended to be used for initial context setup.
//
// Arguments:
// ctx - pointer to a context structure created by 
//       previous call to conway_new()
void conway_unset_cell(conway_ctx_t* ctx, size_t row_idx, size_t col_idx);

// conway_simulate_n()
//
// Run the simulation specified by the given context.
// The simulation runs for `n` steps.
//
// Arguments:
// ctx - pointer to a context structure created by 
//       previous call to conway_new()
// n   - the number of steps to simulate.
void conway_simulate_n(conway_ctx_t* ctx, size_t n);

// conway_print_board()
//
// Print the current state of the grid specified 
// by the context structure to stdout.
//
// Arguments:
// ctx - pointer to a context structure created by 
//       previous call to conway_new()
void conway_print_grid(conway_ctx_t* ctx);

#endif
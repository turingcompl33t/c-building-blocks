// conway.c
// Implementation of John Conway's Game of Life cellular automaton.

#include "conway.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// ----------------------------------------------------------------------------
// Internal Declarations

static const size_t N_ROWS_MIN = 5;
static const size_t N_COLS_MIN = 5;

// Arbitrary choice for the representation of a single cell.
// Always either 0 = DEAD, 1 = ALIVE.
typedef uint_fast8_t cell_t;

// The grid is represented as a flat, 2-dimensional array of cells.
typedef uint_fast8_t* grid_t;

struct conway_ctx
{
    // The dimensions of the grid.
    size_t n_rows;
    size_t n_cols;

    // The selector for the current grid. 0 or 1.
    size_t grid_idx;

    // The two grids that represent the state of simulation.
    // We switch between the two grids in each iteration.
    grid_t grids[2];

    // The number of steps simulated.
    size_t n_steps;
};

static void simulate_tick(conway_ctx_t* ctx);
static size_t compute_neighbors(
    conway_ctx_t* ctx, 
    size_t x, 
    size_t y);

static cell_t get_cell(grid_t grid, size_t n_cols, size_t x, size_t y);
static void set_cell(grid_t grid, size_t n_cols, size_t x, size_t y);
static void unset_cell(grid_t grid, size_t n_cols, size_t x, size_t y);

// ----------------------------------------------------------------------------
// Exported

conway_ctx_t* conway_new(size_t n_rows, size_t n_cols)
{
    if (n_rows < N_ROWS_MIN || n_cols < N_COLS_MIN)
    {
        return NULL;
    }

    conway_ctx_t* ctx = malloc(sizeof(conway_ctx_t));
    if (NULL == ctx)
    {
        return NULL;
    }

    const size_t grid_size = n_rows*n_cols;

    grid_t grid_0 = calloc(grid_size, sizeof(cell_t));
    grid_t grid_1 = calloc(grid_size, sizeof(cell_t));
    if (NULL == grid_0 || NULL == grid_1)
    {
        if (NULL == grid_0) free(grid_0);
        if (NULL == grid_1) free(grid_1);

        free(ctx);
        return NULL;
    }

    ctx->grids[0] = grid_0;
    ctx->grids[1] = grid_1;

    ctx->n_rows   = n_rows;
    ctx->n_cols   = n_cols;
    ctx->grid_idx = 0;
    ctx->n_steps  = 0;

    return ctx;
}

void conway_delete(conway_ctx_t* ctx)
{
    if (NULL == ctx)
    {
        return;
    }

    if (ctx->grids[0] != NULL)
    {
        free(ctx->grids[0]);
    }

    if (ctx->grids[1] != NULL)
    {
        free(ctx->grids[0]);
    }

    free(ctx);
}

void conway_set_cell(conway_ctx_t* ctx, size_t x, size_t y)
{
    if (NULL == ctx)
    {
        return;
    }

    // out of range
    if (x >= ctx->n_cols || y >= ctx->n_rows)
    {
        return;
    }

    grid_t grid = ctx->grids[ctx->grid_idx];
    set_cell(grid, ctx->n_cols, x, y);
}

void conway_unset_cell(conway_ctx_t* ctx, size_t x, size_t y)
{
    if (NULL == ctx)
    {
        return;
    }

    // out of range
    if (x >= ctx->n_cols || y >= ctx->n_rows)
    {
        return;
    }

    grid_t grid = ctx->grids[ctx->grid_idx];
    unset_cell(grid, ctx->n_cols, x, y);
}

void conway_simulate_n(conway_ctx_t* ctx, size_t n)
{
    if (NULL == ctx)
    {
        return;
    }

    for (size_t tick = 0; tick < n; ++tick)
    {
        simulate_tick(ctx);
    }
}

void conway_print_grid(conway_ctx_t* ctx)
{
    // determine the correct grid to print
    grid_t grid = ctx->grids[ctx->grid_idx];
    
    const size_t n_rows = ctx->n_rows;
    const size_t n_cols = ctx->n_cols;

    for (size_t i = 0; i < n_rows; ++i) 
    {
        for (size_t j = 0; j < n_cols; ++j) 
        {
            if (0 == get_cell(grid, n_cols, j, i)) 
            {
                printf(". ");
            } 
            else 
            {
                printf("x ");
            }
        }
        puts("");    
    }
    puts("");
}

// ----------------------------------------------------------------------------
// Internal

// Progress the simulation by a single tick.
static void simulate_tick(conway_ctx_t* ctx)
{
    // determine the current grid
    const size_t src_idx = ctx->grid_idx;
    const size_t dst_idx = ctx->grid_idx ^ 1;

    grid_t src_grid = ctx->grids[src_idx];
    grid_t dst_grid = ctx->grids[dst_idx];

    const size_t n_rows = ctx->n_rows;
    const size_t n_cols = ctx->n_cols;

    // iterate over rows
    for (size_t i = 0; i < n_rows; ++i) 
    {
        // iterate over columns
        for (size_t j = 0; j < n_cols; ++j) 
        {
            // get the neighbors for this cell
            const size_t n = compute_neighbors(ctx, j, i);

            if (0 == get_cell(src_grid, ctx->n_cols, j, i))
            {
                // this cell is currently dead
                if (3 == n) 
                {
                    // but has exactly 3 live neighbors, so it becomes alive
                    set_cell(dst_grid, n_cols, j, i);
                }
                else 
                {
                    // otherwise, make sure it stays dead (artifact of mutliple grids)
                    unset_cell(dst_grid, n_cols, j, i);
                }
            } 
            else 
            {
                // this cell is currently alive
                if (n < 2) 
                {
                    // underpopulation, dies
                    unset_cell(dst_grid, n_cols, j, i);
                } 
                else if (2 == n|| 3 == n) 
                {
                    // goldilocks, lives
                    set_cell(dst_grid, n_cols, j, i);
                } 
                else 
                {
                    // overpopulation, dies
                    unset_cell(dst_grid, n_cols, j, i);
                }
            }
        }
    }

    // update the current grid index
    ctx->grid_idx = dst_idx;
}

// Compute the count of neighbors for a given cell.
static size_t compute_neighbors(
    conway_ctx_t* ctx, 
    size_t x, 
    size_t y) 
{
    grid_t grid = ctx->grids[ctx->grid_idx];

    const size_t max_x = ctx->n_cols;
    const size_t max_y = ctx->n_rows;

    // grid is a torus, need to account for wrapping
    const size_t up    = (y == 0) ? max_y - 1 : y - 1;
    const size_t down  = (y >= (max_y - 1)) ? 0 : y + 1;
    const size_t left  = (x == 0) ? max_x - 1 : x - 1;
    const size_t right = (x >= (max_x - 1)) ? 0 : x + 1;

    const size_t n_cols = ctx->n_cols;

    size_t n = 0;
    n += get_cell(grid, n_cols, left, up);
    n += get_cell(grid, n_cols, x, up);
    n += get_cell(grid, n_cols, right, up);
    n += get_cell(grid, n_cols, left, y);
    n += get_cell(grid, n_cols, right, y);
    n += get_cell(grid, n_cols, left, down);
    n += get_cell(grid, n_cols, x, down);
    n += get_cell(grid, n_cols, right, down);

    return n;
}

// Get the cell at location (x, y).
static cell_t get_cell(grid_t grid, size_t n_cols, size_t x, size_t y)
{    
    const size_t cell_index = y*n_cols + x;
    return grid[cell_index];
}

// Set the cell at location (x, y).
static void set_cell(grid_t grid, size_t n_cols, size_t x, size_t y)
{    
    const size_t cell_index = y*n_cols + x;
    grid[cell_index] = 1;
}

// Unset the cell at location (x, y).
static void unset_cell(grid_t grid, size_t n_cols, size_t x, size_t y)
{    
    const size_t cell_index = y*n_cols + x;
    grid[cell_index] = 0;
}
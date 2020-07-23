// conway.c
// Implementation of John Conway's Game of Life cellular automaton.

#include "conway.h"

conway_ctx_t* conway_new(size_t n_rows, size_t n_cols)
{
    return NULL;
}

void conway_delete(conway_ctx_t* ctx)
{
    return;
}

void conway_set_cell(conway_ctx_t* ctx, size_t row_idx, size_t col_idx)
{
    return;
}

void conway_unset_cell(conway_ctx_t* ctx, size_t row_idx, size_t col_idx)
{
    return;
}

void conway_simulate_n(conway_ctx_t* ctx, size_t n)
{
    return;
}

void conway_print_grid(conway_ctx_t* ctx)
{
    return;
}

int board0[HEIGHT][WIDTH];
int board1[HEIGHT][WIDTH];

void initialize_board();

void step_board(int iter);
void step_board0();
void step_board1();

int neighbors_board0(int x, int y);
int neighbors_board1(int x, int y);

void print_board(int iter);
void print_board0();
void print_board1();

int main() {
  // setup the initial state
  initialize_board();

  // hardcoded setup (glider)
  board0[10][9] = 1;
  board0[10][10] = 1;
  board0[10][11] = 1;
  board0[9][11] = 1;
  board0[8][10] = 1;

  int iter = 0;

  // game loop
  while (true) {
    print_board(iter);
    step_board(iter);

    iter = (iter + 1) % 2;

    sleep(1);
  }

  return 0;
}

/*
 * Initialize the board array.
 * Always updates board 0 by default.
 */
void initialize_board() {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      board0[i][j] = 0;
    }
  }
}

/*
 * Step the board forward a single tick.
 */
void step_board(int iter) {
  if (iter == 0) {
    step_board0();
  } else {
    step_board1();
  }
}

/*
 * Step forward from board0
 * --> updates into board1
 */
void step_board0() {
  // iterate over rows
  for (int i = 0; i < HEIGHT; i++) {
    // iterate over columns
    for (int j = 0; j < WIDTH; j++) {
      // get the neighbors for this cell
      int n = neighbors_board0(j, i);

      if (board0[i][j] == 0) {
        // this cell is currently dead
        if (n == 3) {
          // goldilocks --> lives
          board1[i][j] = 1;
        }
      } else {
        // this cell is currently alive
        if (n < 2) {
          // underpopulation --> dies
          board1[i][j] = 0;
        } else if (n == 2 || n == 3) {
          // goldilocks --> lives
          board1[i][j] = 1;
        } else {
          // overpopulation --> dies
          board1[i][j] = 0;
        }
      }
    }
  }
}

/*
 * Compute neighbors for given cell, in board0.
 */
int neighbors_board0(int x, int y) {
  int up;
  int down;
  int left;
  int right;

  if (y >= (HEIGHT - 1)) {
    down = 0;
  } else {
    down = y + 1;
  }

  if (y <= 0) {
    up = HEIGHT - 1;
  } else {
    up = y - 1;
  }

  if (x >= (WIDTH - 1)) {
    right = 0;
  } else {
    right = x + 1;
  }

  if (x <= 0) {
    left = WIDTH - 1;
  } else {
    left = x - 1;
  }

  int n = 0;
  n += board0[up][left];
  n += board0[up][x];
  n += board0[up][right];
  n += board0[y][left];
  n += board0[y][right];
  n += board0[down][left];
  n += board0[down][x];
  n += board0[down][right];

  return n;
}

/*
 * Step forward from board1
 * --> updates into board0
 */
 void step_board1() {
   // iterate over rows
   for (int i = 0; i < HEIGHT; i++) {
     // iterate over columns
     for (int j = 0; j < WIDTH; j++) {
       // get the neighbors for this cell
       int n = neighbors_board1(j, i);

       if (board1[i][j] == 0) {
         // this cell is currently dead
         if (n == 3) {
           // goldilocks --> lives
           board0[i][j] = 1;
         }
       } else {
         // this cell is currently alive
         if (n < 2) {
           // underpopulation --> dies
           board0[i][j] = 0;
         } else if (n == 2 || n == 3) {
           // goldilocks --> lives
           board0[i][j] = 1;
         } else {
           // overpopulation --> dies
           board0[i][j] = 0;
         }
       }
     }
   }
 }

 /*
  * Compute neighbors for given cell, in board1.
  */
 int neighbors_board1(int x, int y) {
   int up;
   int down;
   int left;
   int right;

   if (y >= (HEIGHT - 1)) {
     down = 0;
   } else {
     down = y + 1;
   }

   if (y <= 0) {
     up = HEIGHT - 1;
   } else {
     up = y - 1;
   }

   if (x >= (WIDTH - 1)) {
     right = 0;
   } else {
     right = x + 1;
   }

   if (x <= 0) {
     left = WIDTH - 1;
   } else {
     left = x - 1;
   }

   int n = 0;
   n += board1[up][left];
   n += board1[up][x];
   n += board1[up][right];
   n += board1[y][left];
   n += board1[y][right];
   n += board1[down][left];
   n += board1[down][x];
   n += board1[down][right];

   return n;
 }

/*
 * Wrapper to print board to display based on iteration.
 */
void print_board(int iter) {
  if (iter == 0) {
    print_board0();
  } else {
    print_board1();
  }
}

/*
 * Print board0 to the display.
 */
void print_board0() {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (board0[i][j] == 0) {
        printf(". ");
      } else {
        printf("x ");
      }
    }
    printf("\n");
  }
  printf("\n");
}

/*
 * Print board1 to the display.
 */
void print_board1() {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (board1[i][j] == 0) {
        printf(". ");
      } else {
        printf("x ");
      }
    }
    printf("\n");
  }
  printf("\n");
}
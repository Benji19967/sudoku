#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../data_structures/ds_c/src/array.h"

#define STRING_BUFFER_SIZE 200

const int EMPTY = 0;

typedef struct Pair {
  int i;
  int j;
} Pair;

int** sudoku_read_board(const char* filepath, const int n) {
  // TODO: Would it be a better design decision to ask users to pass in an array
  // for board?
  // That way they know they have to take care of allocating/deallocating
  // memory.

  FILE* fptr = NULL;
  // filepath is relative to cwd (i.e. dir where program was launched)
  fptr = fopen(filepath, "r");
  if (fptr == NULL) {
    fclose(fptr);
    return NULL;
  }
  char string_buffer[STRING_BUFFER_SIZE];
  int** board = array_create_2d(n, n);
  char* token;

  if (fptr != NULL) {
    int i = 0;
    // one call to fscanf reads one line
    while (fscanf(fptr, "%s", string_buffer) != EOF) {
      token = strtok(string_buffer, ",");
      int j = 0;
      while (token) {
        if (*token == 'x') {
          board[i][j] = EMPTY;
        } else {
          board[i][j] = atoi(token);
        }
        token = strtok(NULL, ",");
        j++;
      }
      i++;
    }
  }
  fclose(fptr);

  return board;
}

int ok_to_place(int** board, const int n, int i, int j, int number) {
  int sqrt_n = sqrt(n);

  // horizontal
  int row = i;
  for (int col = 0; col < n; col++) {
    if (board[row][col] == number && col != j) {
      return 0;
    }
  }

  // vertical
  int col = j;
  for (int row = 0; row < n; row++) {
    if (board[row][col] == number && row != i) {
      return 0;
    }
  }

  // bloc
  int top_left_row = sqrt_n * (i / sqrt_n);
  int top_left_col = sqrt_n * (j / sqrt_n);
  for (int d_row = 0; d_row < sqrt_n; d_row++) {
    row = top_left_row + d_row;
    for (int d_col = 0; d_col < sqrt_n; d_col++) {
      col = top_left_col + d_col;
      if (board[row][col] == number && row != i && col != j) {
        return 0;
      }
    }
  }

  return 1;
}

/*
 * Get bloc number of a cell.
 *
 * Example indexing for a 9x9 board:
 *
 * 0 1 2
 * 3 4 5
 * 6 7 8
 */
int sudoku_get_bloc_index(const int n, int i, int j) {
  int sqrt_n = sqrt(n);
  int bloc_idx_vertical = i / sqrt_n;
  int bloc_idx_horizontal = j / sqrt_n;

  return bloc_idx_vertical * sqrt_n + bloc_idx_horizontal;
}

/*
 * If a number appears more than once in a row, col, or bloc, then the board is
 * invalid.
 */
bool sudoku_is_board_invalid(int** board, const int n) {
  int rows[n][n + 1];
  int cols[n][n + 1];
  int blocs[n][n + 1];
  array_init_2d((int*)rows, n, n + 1, 0);
  array_init_2d((int*)cols, n, n + 1, 0);
  array_init_2d((int*)blocs, n, n + 1, 0);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      int num = board[i][j];
      int bloc_idx = sudoku_get_bloc_index(n, i, j);

      rows[i][num]++;
      cols[j][num]++;
      blocs[bloc_idx][num]++;
    }
  }

  for (int i = 0; i < n; i++) {
    for (int num = 1; num < n + 1; num++) {
      if (rows[i][num] > 1 || cols[i][num] > 1 || blocs[i][num] > 1) {
        return true;
      }
    }
  }

  return false;
}

bool sudoku_is_board_full(int** board, const int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (board[i][j] == EMPTY) {
        return false;
      }
    }
  }
  return true;
}

bool sudoku_is_board_valid(int** board, const int n) {
  if (sudoku_is_board_full(board, n) && !sudoku_is_board_invalid(board, n)) {
    return true;
  }
  return false;
}

Pair next_cell(const int n, int i, int j) {
  Pair p;
  if (j == n - 1) {
    p.i = i + 1;
    p.j = 0;
  } else {
    p.i = i;
    p.j = j + 1;
  }
  return p;
}

int try_to_place(int** board, const int n, int** solutions[], int solution_idx,
                 const int max_solutions, int i, int j) {
  if (board[i][j] != EMPTY) {
    if (i == n - 1 && j == n - 1) {
      solutions[solution_idx] = array_copy_2d(board, n, n);
      solution_idx = solution_idx + 1;
      board[i][j] = EMPTY;
      return solution_idx;
    }
    Pair new_cell = next_cell(n, i, j);
    return try_to_place(board, n, solutions, solution_idx, max_solutions,
                        new_cell.i, new_cell.j);
  }

  for (int number = 1; number < n + 1; number++) {
    if (ok_to_place(board, n, i, j, number)) {
      board[i][j] = number;
      if (i == n - 1 && j == n - 1) {
        solutions[solution_idx] = array_copy_2d(board, n, n);
        solution_idx = solution_idx + 1;
        board[i][j] = EMPTY;
        return solution_idx;
      }
      Pair new_cell = next_cell(n, i, j);
      solution_idx = try_to_place(board, n, solutions, solution_idx,
                                  max_solutions, new_cell.i, new_cell.j);
      if (solution_idx >= max_solutions) {
        return solution_idx;
      }
      board[i][j] = EMPTY;
    }
  }
  return solution_idx;
}

int** sudoku_solve(int** input_board, const int n) {
  // TODO: Same as above: should probably ask user to pass an empty board so
  // they know to deallocate?
  // TODO: Maybe, for consistency, make the API similar to solve_all where the
  // users needs to pass a placeholder for the solution (here: int** solution).

  int** board = array_copy_2d(input_board, n, n);
  int** solutions[1];
  int num_solutions = try_to_place(board, n, solutions, 0, 1, 0, 0);
  if (num_solutions > 0) {
    int** solution = array_create_2d(n, n);
    solution = solutions[0];
    return solution;
  }
  return NULL;
}

int sudoku_solve_all(int** input_board, const int n, int** solutions[],
                     const int max_solutions) {
  // TODO: Same as above: should probably ask user to pass an empty board so
  // they know to deallocate?

  int** board = array_copy_2d(input_board, n, n);
  int num_solutions = try_to_place(board, n, solutions, 0, max_solutions, 0, 0);
  return num_solutions;
}

/*
 * min/max are both inclusive
 */
int random_int(int min, int max) {
  return rand() % (max - min + 1) + min;
}

int** sudoku_generate_solved_board(int** initial_board,
                                   const int initial_number_to_place,
                                   const int n) {
  // TODO: There are probably more efficient ways to achieve this. The current
  // solution is quite brute force.

  int** solved_board = NULL;
  int** board = NULL;
  while (solved_board == NULL) {
    board = array_create_2d(n, n);

    int numbers_placed = 0;
    while (numbers_placed < initial_number_to_place) {
      int min_number = 1;
      int max_number = n;
      int number = random_int(min_number, max_number);

      int min_idx = 0;
      int max_idx = n - 1;
      int i = random_int(min_idx, max_idx);
      int j = random_int(min_idx, max_idx);

      if (board[i][j] == EMPTY && ok_to_place(board, n, i, j, number)) {
        board[i][j] = number;
        numbers_placed++;
      }
    }
    int** board_to_solve = array_copy_2d(board, n, n);
    solved_board = sudoku_solve(board_to_solve, n);
  }
  array_copy_into_dst_2d(board, initial_board, n, n);

  return solved_board;
}

int** sudoku_generate_board(const int number_of_empty_cells, const int n) {
  // TODO: can get stuck when number of empty cells is too large

  srand(time(0));
  int total_number_of_cells = n * n;
  int initial_number_to_place = total_number_of_cells - number_of_empty_cells;
  int** initial_board = array_create_2d(n, n);
  sudoku_generate_solved_board(initial_board, initial_number_to_place, n);

  return initial_board;
}

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../data_structures/ds_c/src/array.h"

#define STRING_BUFFER_SIZE 200

const int EMPTY = 0;

typedef struct Pair {
  int i;
  int j;
} Pair;

int** sudoku_read_board(char* filepath, const int n) {
  // TODO: Would it be a better design decision to ask users to pass in an array
  // for board?
  // That way they know they have to take care of allocating/deallocating
  // memory.

  FILE* fptr = NULL;
  // filepath is relative to cwd (i.e. dir where program was launched)
  fptr = fopen(filepath, "r");
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
 * Check that each row, col, and bloc contains the numbers 1 through n exactly
 * once.
 */
bool sudoku_is_board_valid(int** board, const int n) {
  int rows[n][n + 1];
  int cols[n][n + 1];
  int blocs[n][n + 1];

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      int num = board[i][j];
      int bloc_idx = sudoku_get_bloc_index(n, i, j);

      rows[i][num] = 1;
      cols[j][num] = 1;
      blocs[bloc_idx][num] = 1;
    }
  }

  for (int i = 0; i < n; i++) {
    for (int num = 1; num < n + 1; num++) {
      if (rows[i][num] != 1 || cols[i][num] != 1 || blocs[i][num] != 1) {
        return false;
      }
    }
  }

  return true;
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
                 int max_solutions, int i, int j) {
  // printf("Start i: %d, j: %d, solution_idx: %d\n", i, j, solution_idx);
  if (board[i][j] != EMPTY) {
    if (i == n - 1 && j == n - 1) {
      // printf("Found solution 0\n");
      solutions[solution_idx] = array_copy_2d(board, 9, 9);
      solution_idx = solution_idx + 1;
      printf("return 0: %d\n", solution_idx);
      board[i][j] = EMPTY;
      return solution_idx;
    }
    Pair new_cell = next_cell(n, i, j);
    // return try_to_place(board, solutions, solution_idx, new_cell.i,
    // new_cell.j);
    return try_to_place(board, n, solutions, solution_idx, max_solutions,
                        new_cell.i, new_cell.j);
    printf("Post return 0\n");
    // if (solution_idx >= max_solutions) {
    //   return solution_idx;
    // }
  }

  for (int number = 1; number < n + 1; number++) {
    if (ok_to_place(board, n, i, j, number)) {
      board[i][j] = number;
      if (i == n - 1 && j == n - 1) {
        // printf("Found solution 1\n");
        solutions[solution_idx] = array_copy_2d(board, 9, 9);
        solution_idx = solution_idx + 1;
        printf("return 1: %d\n", solution_idx);
        board[i][j] = EMPTY;
        return solution_idx;
      }
      Pair new_cell = next_cell(n, i, j);
      // int found_solution = try_to_place(board, new_cell.i, new_cell.j);
      // if (found_solution) {
      //   return 1;
      // }
      printf("try_to_place i: %d, j: %d, number: %d, solution_idx: %d\n", i, j,
             number, solution_idx);
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

  int** board = array_copy_2d(input_board, n, n);
  // int found_solution = try_to_place(board, 0, 0);

  int** solutions[100];  // TODO: parametrize
  try_to_place(board, n, solutions, 0, 1, 0, 0);
  // if (found_solution) {
  //   return board;
  // }
  // return NULL;
  int** solution = array_create_2d(9, 9);
  solution = solutions[0];
  return solution;
}

void sudoku_solve_all(int** input_board, const int n, int** solutions[]) {
  // TODO: Same as above: should probably ask user to pass an empty board so
  // they know to deallocate?

  int** board = array_copy_2d(input_board, n, n);
  printf("Start try_to_place solve_all\n");
  try_to_place(board, n, solutions, 0, 20, 0, 0);
  // return solutions;
}

int** sudoku_generate_solved_board(const int n) {
  int** board = array_create_2d(n, n);

  return sudoku_solve(board, n);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../data_structures/ds_c/src/array.h"

#define STRING_BUFFER_SIZE 200

const int N = 9;
const int N_SQRT = 3;
const int EMPTY = 0;

typedef struct Pair {
  int i;
  int j;
} Pair;

int** sudoku_read_board(char* filepath) {
  // TODO: Would it be a better design decision to ask users to pass in an array
  // for board?
  // That way they know they have to take care of allocating/deallocating
  // memory.

  FILE* fptr = NULL;
  // filepath is relative to cwd (i.e. dir where program was launched)
  fptr = fopen(filepath, "r");
  char string_buffer[STRING_BUFFER_SIZE];
  int** board = array_create_2d(N, N);
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

int ok_to_place(int** board, int i, int j, int number) {
  // horizontal
  int row = i;
  for (int col = 0; col < N; col++) {
    if (board[row][col] == number && col != j) {
      return 0;
    }
  }

  // vertical
  int col = j;
  for (int row = 0; row < N; row++) {
    if (board[row][col] == number && row != i) {
      return 0;
    }
  }

  // bloc
  int top_left_row = N_SQRT * (i / N_SQRT);
  int top_left_col = N_SQRT * (j / N_SQRT);
  for (int d_row = 0; d_row < N_SQRT; d_row++) {
    row = top_left_row + d_row;
    for (int d_col = 0; d_col < N_SQRT; d_col++) {
      col = top_left_col + d_col;
      if (board[row][col] == number && row != i && col != j) {
        return 0;
      }
    }
  }

  return 1;
}

Pair next_cell(int i, int j) {
  Pair p;
  if (j == N - 1) {
    p.i = i + 1;
    p.j = 0;
  } else {
    p.i = i;
    p.j = j + 1;
  }
  return p;
}

int try_to_place(int** board, int i, int j) {
  if (board[i][j] != EMPTY) {
    if (i == N - 1 && j == N - 1) {
      return 1;
    }
    Pair new_cell = next_cell(i, j);
    return try_to_place(board, new_cell.i, new_cell.j);
  }

  for (int number = 1; number < N + 1; number++) {
    if (ok_to_place(board, i, j, number)) {
      board[i][j] = number;
      if (i == N - 1 && j == N - 1) {
        return 1;
      }
      Pair new_cell = next_cell(i, j);
      int found_solution = try_to_place(board, new_cell.i, new_cell.j);
      if (found_solution) {
        return 1;
      }
      board[i][j] = EMPTY;
    }
  }
  return 0;
}

int** sudoku_solve(int** input_board) {
  int** board = array_copy_2d(input_board, 9, 9);
  int found_solution = try_to_place(board, 0, 0);
  if (found_solution) {
    return board;
  }
  return NULL;
}

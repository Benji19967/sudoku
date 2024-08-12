#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../data_structures/ds_c/src/array.h"

#define STRING_BUFFER_SIZE 200

const int N = 9;

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
        board[i][j] = atoi(token);
        token = strtok(NULL, ",");
        j++;
      }
      i++;
    }
  }
  fclose(fptr);

  return board;
}

// void sudoku(int board[][n]) {
//   // TODO
// }

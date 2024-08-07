#include <stdio.h>

#define STRING_BUFFER_SIZE 200

const int n = 9;

int* sudoku_read_board(char* filepath) {
  FILE* fptr = NULL;
  fptr = fopen(filepath, "r");
  char string_buffer[STRING_BUFFER_SIZE];

  // one call to fscanf reads one line
  if (fptr != NULL) {
    int i = 0;
    // TODO: parse line correctly
    // while (fscanf(fptr, "%s", string_buffer) != EOF) {
    while (fscanf(fptr, "%d,%d,%d,%d,%d,%d,%d,%d,%d", string_buffer) != EOF) {
    }
  }

  fclose(fptr);

  void sudoku(int board[][n]) {
    // TODO
  }

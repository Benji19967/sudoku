#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_BUFFER_SIZE 200

const int n = 9;

int* sudoku_read_board(char* filepath) {
  FILE* fptr = NULL;
  fptr = fopen(filepath, "r");
  char string_buffer[STRING_BUFFER_SIZE];
  int lines[n][n];

  // one call to fscanf reads one line
  if (fptr != NULL) {
    // TODO: parse line correctly
    int i = 0;
    while (fscanf(fptr, "%s", string_buffer) != EOF) {
      char* token = strtok(string_buffer, ",");
      int j = 0;
      while (token) {
        lines[i][j] = atoi(token);
        token = strtok(NULL, ",");
        j++;
      }
      i++;
    }
  }

  fclose(fptr);

  void sudoku(int board[][n]) {
    // TODO
  }

#include "sudoku.h"

// TODO: Add logging
// TODO: Raise an error if input file does not exist

int main(int argc, char* argv[]) {
  char* filepath = argv[1];
  int** input_board = sudoku_read_board(filepath);
  sudoku_solve(input_board);
}

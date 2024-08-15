#include "sudoku.h"

int main(int argc, char* argv[]) {
  char* filepath = argv[1];
  int** input_board = sudoku_read_board(filepath);
  sudoku_solve(input_board);
}

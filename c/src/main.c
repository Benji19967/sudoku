#include "sudoku.h"

int main(int argc, char* argv[]) {
  int** input_board = sudoku_read_board("../boards/input/004_5.csv");
  sudoku_solve(input_board);
}

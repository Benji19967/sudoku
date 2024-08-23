#include <stdio.h>

#include "../data_structures/ds_c/src/array.h"
#include "sudoku.h"

// TODO: Add logging
// TODO: Raise an error if input file does not exist
//

/*
 * CLI libraries in C:
 *  - getopt (GNU)
 *  - argparse (https://github.com/cofyc/argparse)
 */

int main(int argc, char* argv[]) {
  char* filepath = argv[1];
  int** input_board = sudoku_read_board(filepath, 9);
  int** solved_board = sudoku_solve(input_board, 9);
  if (solved_board) {
    array_print_2d(solved_board, 9, 9);
  } else {
    printf("Did not find a solution for input board:\n");
    array_print_2d(input_board, 9, 9);
  }
}

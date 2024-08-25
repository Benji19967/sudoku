#include <stdio.h>
#include <time.h>

#include "../argparse/argparse.h"
#include "../data_structures/ds_c/src/array.h"
#include "sudoku.h"

// TODO: Add logging
// TODO: Raise an error if input file does not exist

static const char* const usages[] = {
    "basic [options] [[--] args]",
    "basic [options]",
    NULL,
};

/*
 * CLI libraries in C:
 *  - getopt (GNU)
 *  - argparse (https://github.com/cofyc/argparse)
 */
int main(int argc, const char** argv) {
  const char* path = NULL;
  int n = 9;

  struct argparse_option options[] = {
      OPT_HELP(),
      OPT_GROUP("Basic options"),
      OPT_STRING('p', "path", &path, "path to board", NULL, 0, 0),
      OPT_INTEGER('n', "int", &n, "board size", NULL, 0, 0),
      OPT_END(),
  };

  struct argparse argparse;
  argparse_init(&argparse, options, usages, 0);
  argparse_describe(&argparse,
                    "\nSolve sudoku puzzles of generate new sudoku boards",
                    "\nEnjoy!");
  argc = argparse_parse(&argparse, argc, argv);

  if (path == NULL) {
    printf("Please specify a path to a board to solve\n");
    return -1;
  }
  int** board = sudoku_read_board(path, n);
  if (board == NULL) {
    printf("Path to board '%s' is invalid\n", path);
    return -1;
  }
  int** solved_board = sudoku_solve(board, n);

  array_print_2d(solved_board, n, n);

  return 0;
}

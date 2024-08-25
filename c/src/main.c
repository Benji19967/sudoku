#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../argparse/argparse.h"
#include "../data_structures/ds_c/src/array.h"
#include "sudoku.h"

// TODO: Add logging
// TODO: Raise an error if input file does not exist

/*
 * CLI libraries in C:
 *  - getopt (GNU)
 *  - argparse (https://github.com/cofyc/argparse)
 */

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

static const char *const usages[] = {
    "subcommands [options] [cmd] [args]",
    NULL,
};

struct cmd_struct {
  const char *cmd;
  int (*fn)(int, const char **);
};

int cmd_solve(int argc, const char **argv) {
  const char *path = NULL;
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
  argc = argparse_parse(&argparse, argc, argv);

  if (path == NULL) {
    printf("Please specify a path to a board to solve\n");
    return -1;
  }
  int **board = sudoku_read_board(path, n);
  if (board == NULL) {
    printf("Path to board '%s' is invalid\n", path);
    return -1;
  }
  int **solved_board = sudoku_solve(board, n);

  array_print_2d(solved_board, n, n);

  return 0;
}

int cmd_generate(int argc, const char **argv) {
  int n = 9;
  int number_of_empty_cells = 0;
  struct argparse_option options[] = {
      OPT_HELP(),
      OPT_GROUP("Basic options"),
      OPT_INTEGER('n', "int", &n, "board size", NULL, 0, 0),
      OPT_INTEGER('e', "int", &number_of_empty_cells, "number of empty cells",
                  NULL, 0, 0),
      OPT_END(),
  };
  struct argparse argparse;
  argparse_init(&argparse, options, usages, 0);
  argc = argparse_parse(&argparse, argc, argv);

  int **board = sudoku_generate_board(number_of_empty_cells, n);

  array_print_2d(board, n, n);

  return 0;
}

static struct cmd_struct commands[] = {
    {"solve", cmd_solve},
    {"generate", cmd_generate},
};

int main(int argc, const char **argv) {
  struct argparse argparse;
  struct argparse_option options[] = {
      OPT_HELP(),
      OPT_END(),
  };
  argparse_init(&argparse, options, usages, ARGPARSE_STOP_AT_NON_OPTION);
  argc = argparse_parse(&argparse, argc, argv);
  if (argc < 1) {
    argparse_usage(&argparse);
    return -1;
  }

  /* Try to run command with args provided. */
  struct cmd_struct *cmd = NULL;
  for (int i = 0; i < ARRAY_SIZE(commands); i++) {
    if (!strcmp(commands[i].cmd, argv[0])) {
      cmd = &commands[i];
    }
  }
  if (cmd) {
    return cmd->fn(argc, argv);
  }
  return 0;
}

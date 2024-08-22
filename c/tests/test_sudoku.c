#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../data_structures/ds_c/src/array.h"
#include "../src/sudoku.h"
#include "../unity/src/unity.h"

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_sudoku_read_board_invalid_filepath() {
  int** board = sudoku_read_board("does/not/exist", 9);
  TEST_ASSERT_EQUAL_PTR(NULL, board);
}

int** read_board_or_fail(char* filepath, const int n) {
  int** board = sudoku_read_board(filepath, n);
  TEST_ASSERT_NOT_NULL_MESSAGE(board, "Could not read board from filepath");

  return board;
}

void test_sudoku_read_board() {
  int expected_board[9][9] = {
      {1, 2, 3, 4, 5, 6, 7, 8, 9}, {1, 2, 3, 4, 5, 6, 7, 8, 9},
      {1, 2, 3, 4, 5, 6, 7, 8, 9}, {1, 2, 3, 4, 5, 6, 7, 8, 9},
      {1, 2, 3, 4, 5, 6, 7, 8, 9}, {1, 2, 3, 4, 5, 6, 7, 8, 9},
      {1, 2, 3, 4, 5, 6, 7, 8, 9}, {0, 2, 3, 4, 5, 6, 7, 8, 9},
      {9, 8, 7, 6, 5, 4, 3, 2, 1},
  };
  int expected_board_2x2[4][4] = {
      {1, 2, 3, 4},
      {1, 2, 3, 4},
      {0, 2, 3, 4},
      {4, 3, 2, 1},
  };

  int** board =
      read_board_or_fail("tests/fixtures/mock_board_read_input.csv", 9);

  for (int i = 0; i < 9; i++) {
    TEST_ASSERT_EQUAL_INT_ARRAY(expected_board[i], board[i], 9);
  }
  int** board_2x2 =
      read_board_or_fail("tests/fixtures/mock_board_read_input_2x2.csv", 4);

  for (int i = 0; i < 4; i++) {
    TEST_ASSERT_EQUAL_INT_ARRAY(expected_board_2x2[i], board_2x2[i], 4);
  }

  array_delete_2d(board, 9, 9);
  array_delete_2d(board_2x2, 4, 4);
}

void test_sudoku_is_board_valid() {
  // TODO: clean this test up. Can you add parametrized tests?
  char* board_filepath = "tests/fixtures/mock_board_is_valid.csv";
  int** board = read_board_or_fail(board_filepath, 9);
  bool is_board_valid = sudoku_is_board_valid(board, 9);

  char* board_filepath1 = "tests/fixtures/mock_board_is_invalid.csv";
  int** board1 = read_board_or_fail(board_filepath1, 9);
  bool is_board_valid1 = sudoku_is_board_valid(board1, 9);

  char* board_filepath_2x2_0 = "tests/fixtures/mock_board_is_valid_2x2.csv";
  int** board_2x2_0 = read_board_or_fail(board_filepath_2x2_0, 4);
  bool is_board_valid_2x2_0 = sudoku_is_board_valid(board_2x2_0, 4);

  char* board_filepath_2x2_1 = "tests/fixtures/mock_board_is_invalid_2x2.csv";
  int** board_2x2_1 = read_board_or_fail(board_filepath_2x2_1, 4);
  bool is_board_valid_2x2_1 = sudoku_is_board_valid(board_2x2_1, 4);

  TEST_ASSERT_TRUE(is_board_valid);
  TEST_ASSERT_FALSE(is_board_valid1);
  TEST_ASSERT_TRUE(is_board_valid_2x2_0);
  TEST_ASSERT_FALSE(is_board_valid_2x2_1);
}

void test_sudoku_solve() {
  char* input_board_filepath = "tests/fixtures/mock_board_input.csv";
  int** input_board = read_board_or_fail(input_board_filepath, 9);
  int** solved_board = sudoku_solve(input_board, 9);

  char* input_board_filepath_2x2 = "tests/fixtures/mock_board_input_2x2.csv";
  int** input_board_2x2 = read_board_or_fail(input_board_filepath_2x2, 4);
  int** solved_board_2x2 = sudoku_solve(input_board_2x2, 4);

  TEST_ASSERT_TRUE(sudoku_is_board_valid(solved_board, 9));
  TEST_ASSERT_TRUE(sudoku_is_board_valid(solved_board_2x2, 4));
}

void assert_each_solution_is_unique(int** solutions[], const int num_solutions,
                                    const int n) {
  for (int i = 0; i < num_solutions - 1; i++) {
    for (int j = i + 1; j < num_solutions; j++) {
      TEST_ASSERT_FALSE(array_is_equal_2d(solutions[i], solutions[j], n, n));
    }
  }
}

void test_sudoku_solve_all() {
  // Can you parametrize this test?
  bool verbose = false;

  // TODO: Add a few more test cases for 2x2
  int max_num_solutions = 500;
  char* input_board_filepath =
      "tests/fixtures/mock_board_non_unique_solution.csv";
  int** input_board = read_board_or_fail(input_board_filepath, 9);
  int** solutions[max_num_solutions];
  int num_solutions =
      sudoku_solve_all(input_board, 9, solutions, max_num_solutions);
  assert_each_solution_is_unique(solutions, num_solutions, 9);
  TEST_ASSERT_EQUAL_INT(max_num_solutions, num_solutions);

  int max_num_solutions_2x2 = 500;
  char* input_board_filepath_2x2 =
      "tests/fixtures/mock_board_non_unique_solution_2x2.csv";
  int** input_board_2x2 = read_board_or_fail(input_board_filepath_2x2, 4);
  int** solutions_2x2[max_num_solutions_2x2];
  int num_solutions_2x2 = sudoku_solve_all(input_board_2x2, 4, solutions_2x2,
                                           max_num_solutions_2x2);
  assert_each_solution_is_unique(solutions_2x2, num_solutions_2x2, 4);
  // TEST_ASSERT_EQUAL_INT(max_num_solutions_2x2, num_solutions_2x2);

  if (verbose) {
    int n = 9;
    for (int i = 0; i < num_solutions; i++) {
      array_print_2d(solutions[i], n, n);
      printf("\n");
    }
    printf("Num solutions: %d\n", num_solutions);
  }
}

void test_sudoku_generate_solved_board() {
  bool verbose = false;

  srand(time(0));
  int n = 4;
  int** initial_board = array_create_2d(n, n);
  int** board = sudoku_generate_solved_board(initial_board, 4, n);

  if (verbose) {
    array_print_2d(board, n, n);
  }

  TEST_ASSERT_TRUE(sudoku_is_board_valid(board, n));
}

void test_sudoku_generate_board() {
  // TODO: Assert only one solution. Or state how many possible solutions there
  // are.
  int n = 4;
  int num_empty_cells = 12;
  int** board = sudoku_generate_board(num_empty_cells, n);
  int** solved_board = sudoku_solve(board, n);

  // TODO: TEST_ASSERT_EQUAL_INT(12, sudoku_num_empty_cells(board, n));
  TEST_ASSERT_TRUE(sudoku_is_board_valid(solved_board, n));
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_sudoku_read_board_invalid_filepath);
  RUN_TEST(test_sudoku_read_board);
  RUN_TEST(test_sudoku_is_board_valid);
  RUN_TEST(test_sudoku_solve);
  RUN_TEST(test_sudoku_solve_all);
  RUN_TEST(test_sudoku_generate_solved_board);
  RUN_TEST(test_sudoku_generate_board);

  UNITY_END();

  return 0;
}

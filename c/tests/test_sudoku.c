#include <stdbool.h>
#include <stdio.h>

#include "../data_structures/ds_c/src/array.h"
#include "../src/sudoku.h"
#include "../unity/src/unity.h"

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
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
      sudoku_read_board("tests/fixtures/mock_board_read_input.csv", 9);

  for (int i = 0; i < 9; i++) {
    TEST_ASSERT_EQUAL_INT_ARRAY(expected_board[i], board[i], 9);
  }
  int** board_2x2 =
      sudoku_read_board("tests/fixtures/mock_board_read_input_2x2.csv", 4);

  for (int i = 0; i < 4; i++) {
    TEST_ASSERT_EQUAL_INT_ARRAY(expected_board_2x2[i], board_2x2[i], 4);
  }

  array_delete_2d(board, 9, 9);
  array_delete_2d(board_2x2, 4, 4);
}

void test_sudoku_is_board_valid() {
  // TODO: clean this test up. Can you add parametrized tests?
  char* board_filepath = "tests/fixtures/mock_board_is_valid_0.csv";
  int** board = sudoku_read_board(board_filepath, 9);
  bool is_board_valid = sudoku_is_board_valid(board, 9);

  char* board_filepath1 = "tests/fixtures/mock_board_is_valid_1.csv";
  int** board1 = sudoku_read_board(board_filepath1, 9);
  bool is_board_valid1 = sudoku_is_board_valid(board1, 9);

  char* board_filepath_2x2_0 = "tests/fixtures/mock_board_is_valid_2x2_0.csv";
  int** board_2x2_0 = sudoku_read_board(board_filepath_2x2_0, 4);
  bool is_board_valid_2x2_0 = sudoku_is_board_valid(board_2x2_0, 4);

  char* board_filepath_2x2_1 = "tests/fixtures/mock_board_is_valid_2x2_1.csv";
  int** board_2x2_1 = sudoku_read_board(board_filepath_2x2_1, 4);
  bool is_board_valid_2x2_1 = sudoku_is_board_valid(board_2x2_1, 4);

  TEST_ASSERT_EQUAL(true, is_board_valid);
  TEST_ASSERT_EQUAL(false, is_board_valid1);
  TEST_ASSERT_EQUAL(true, is_board_valid_2x2_0);
  TEST_ASSERT_EQUAL(false, is_board_valid_2x2_1);
}

void test_sudoku_solve() {
  char* input_board_filepath = "tests/fixtures/mock_board_input.csv";
  int** input_board = sudoku_read_board(input_board_filepath, 9);

  int** solved_board = sudoku_solve(input_board, 9);

  TEST_ASSERT_EQUAL(true, sudoku_is_board_valid(solved_board, 9));
}

void test_sudoku_solve_all() {
  char* input_board_filepath =
      "tests/fixtures/mock_board_non_unique_solution.csv";
  int** input_board = sudoku_read_board(input_board_filepath, 9);
  int** solutions[100];  // TODO: parametrize
  sudoku_solve_all(input_board, 9, solutions);

  int n = 9;
  for (int i = 0; i < n; i++) {
    array_print_2d(solutions[i], n, n);
    printf("\n");
  }
}

void test_sudoku_generate_solved_board() {
  int n = 9;
  int** board = sudoku_generate_solved_board(n);
  // array_print_2d(board, n, n);

  TEST_ASSERT_EQUAL(true, sudoku_is_board_valid(board, n));
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_sudoku_read_board);
  RUN_TEST(test_sudoku_is_board_valid);
  // RUN_TEST(test_sudoku_solve);
  // RUN_TEST(test_sudoku_solve_all);
  // RUN_TEST(test_sudoku_generate_solved_board);

  UNITY_END();

  return 0;
}

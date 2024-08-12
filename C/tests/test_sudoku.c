#include "../data_structures/ds_c/src/array.h"
#include "../src/sudoku.h"
#include "../unity/src/unity.h"

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_read_board() {
  int test_board[9][9] = {
      {1, 2, 3, 4, 5, 6, 7, 8, 9}, {1, 2, 3, 4, 5, 6, 7, 8, 9},
      {1, 2, 3, 4, 5, 6, 7, 8, 9}, {1, 2, 3, 4, 5, 6, 7, 8, 9},
      {1, 2, 3, 4, 5, 6, 7, 8, 9}, {1, 2, 3, 4, 5, 6, 7, 8, 9},
      {1, 2, 3, 4, 5, 6, 7, 8, 9}, {1, 2, 3, 4, 5, 6, 7, 8, 9},
      {9, 8, 7, 6, 5, 4, 3, 2, 1},
  };

  int n = 9;
  int **board = sudoku_read_board("tests/mock_board.csv");
  array_print_2d(board, n, n);

  for (int i = 0; i < n; i++) {
    TEST_ASSERT_EQUAL_INT_ARRAY(test_board[i], board[i], n);
  }

  array_delete_2d(board, n, n);
}

void test_sudoku() {
  // TODO

  TEST_ASSERT_EQUAL_INT(-1, -1);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_read_board);
  RUN_TEST(test_sudoku);

  UNITY_END();

  return 0;
}

#include "../src/sudoku.h"

#include "../unity/src/unity.h"

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_read_board() {
  // TODO: How to init 2d array
  int test_board[9][9] = [
    [ 1, 2, 3, 4, 5, 6, 7, 8, 9 ],
    [ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1 ],
    [ 1, 2, 3, 4, 5, 6, 7, 8, 9 ],
    [ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1 ],
    [ 1, 2, 3, 4, 5, 6, 7, 8, 9 ],
    [ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1 ],
    [ 1, 2, 3, 4, 5, 6, 7, 8, 9 ],
    [ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1 ],
    [ -1, -1, -1, -1, -1, -1, -1, -1, 9 ],
  ];

  int board[9][9] = sudoku_read_board("path/to/file");

  TEST_ASSERT_EQUAL_INT_ARRAY(board, test_board, 9);
}

void test_sudoku() {
  // TODO

  TEST_ASSERT_EQUAL_INT(-1, -1);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_sudoku);

  UNITY_END();

  return 0;
}

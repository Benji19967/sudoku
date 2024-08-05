#include "../src/sudoku.h"

#include "../unity/src/unity.h"

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
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

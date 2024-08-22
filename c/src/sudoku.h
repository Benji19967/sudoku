#include <stdbool.h>

int** sudoku_read_board(char* filepath, const int n);
bool sudoku_is_board_valid(int** board, const int n);
int** sudoku_generate_solved_board(const int initial_numbers_to_place,
                                   const int n);
int** sudoku_solve(int** input_board, const int n);
int sudoku_solve_all(int** input_board, const int n, int** solutions[],
                     const int max_solutions);

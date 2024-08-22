#include <stdbool.h>

int** sudoku_read_board(char* filepath, const int n);
bool sudoku_is_board_valid(int** board, const int n);
int** sudoku_generate_solved_board(int** initial_board,
                                   const int initial_number_to_place,
                                   const int n);
int** sudoku_generate_board(const int number_of_empty_cells, const int n);
int** sudoku_solve(int** input_board, const int n);
int sudoku_solve_all(int** input_board, const int n, int** solutions[],
                     const int max_solutions);

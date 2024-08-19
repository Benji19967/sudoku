#include <stdbool.h>

int** sudoku_read_board(char* filepath);
bool sudoku_is_board_valid(int** board, int n);
int** sudoku_generate_solved_board(int n);
int** sudoku_solve(int** input_board);

import math
from pathlib import Path

from sudoku import io

N = 9
N_SQRT = math.sqrt(N)
EMPTY = -1
EMPTY_CELL_DISPLAY_CHAR = "."


def try_to_place(board, i, j):
    if board[i][j] != EMPTY:
        if i == N - 1 and j == N - 1:
            display_board(board)
            return
        new_i, new_j = next_cell(i, j)
        try_to_place(board, new_i, new_j)
        return

    for number in range(1, N + 1):
        if ok_to_place(board, i, j, number):
            board[i][j] = number
            if i == N - 1 and j == N - 1:
                display_board(board)
                return
            new_i, new_j = next_cell(i, j)
            try_to_place(board, new_i, new_j)
            board[i][j] = EMPTY


def ok_to_place(board, i, j, number) -> bool:
    # horizontal
    row = i
    for col in range(N):
        if board[row][col] == number and col != j:
            return False

    # vertical
    col = j
    for row in range(N):
        if board[row][col] == number and row != i:
            return False

    # bloc
    # TODO: replace '3' with 'sqrt(N)' constant
    def get_top_left_cell() -> tuple[int, int]:
        row = 3 * (i // 3)
        col = 3 * (j // 3)
        return row, col

    top_left_row, top_left_col = get_top_left_cell()
    for d_row in range(3):
        row = top_left_row + d_row
        for d_col in range(3):
            col = top_left_col + d_col
            # print(i, j, top_left_row, top_left_col, d_row, d_col)
            if board[row][col] == number and row != i and col != j:
                return False

    return True


def next_cell(i, j) -> tuple[int, int]:
    if j == N - 1:
        return (i + 1, 0)
    return (i, j + 1)


def display_board(board) -> None:
    # for row in board:
    #     print(" ".join(str(x) for x in row))
    # print("-" * (2 * N - 1))
    len_row_characters = 0

    for row_num, row in enumerate(board):
        row_characters = []
        for col_num, number in enumerate(row):
            if col_num % 3 == 0 and col_num != 0:
                row_characters.append("|")

            # if last_cell_added and last_cell_added == (row_num, col_num):
            #     number = f"\033[91m{cell.number}\033[0m"
            # else:
            #     number = str(cell.number)

            row_characters.append(
                str(number) if number != EMPTY else EMPTY_CELL_DISPLAY_CHAR
            )
            len_row_characters = len(row_characters)
        if row_num % 3 == 0 and row_num != 0:
            print("-" * (2 * len_row_characters - 1))
        print(" ".join(row_characters))
    print()
    print("+" * (2 * len_row_characters - 1))
    print()


def solve(board) -> None:
    try_to_place(board, 0, 0)


if __name__ == "__main__":
    board = [[EMPTY] * N for _ in range(N)]
    board_001 = io.read(filename=Path("inputs/004_5.csv"), empty_number=EMPTY)
    display_board(board_001)
    solve(board_001)

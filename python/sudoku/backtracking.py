import math
from pathlib import Path

from sudoku import io

N = 9
N_SQRT = int(math.sqrt(N))
EMPTY = -1
EMPTY_CELL_DISPLAY_CHAR = "."

Board = list[list[int]]


def try_to_place(board: Board, i: int, j: int) -> None:
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


def ok_to_place(board: Board, i: int, j: int, number: int) -> bool:
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
    def get_top_left_cell() -> tuple[int, int]:
        row = N_SQRT * (i // N_SQRT)
        col = N_SQRT * (j // N_SQRT)
        return row, col

    top_left_row, top_left_col = get_top_left_cell()
    for d_row in range(N_SQRT):
        row = top_left_row + d_row
        for d_col in range(N_SQRT):
            col = top_left_col + d_col
            if board[row][col] == number and row != i and col != j:
                return False

    return True


def next_cell(i: int, j: int) -> tuple[int, int]:
    if j == N - 1:
        return (i + 1, 0)
    return (i, j + 1)


def display_board(board: Board) -> None:
    len_row_characters = 0

    for row_num, row in enumerate(board):
        row_characters = []
        for col_num, number in enumerate(row):
            if col_num % N_SQRT == 0 and col_num != 0:
                row_characters.append("|")
            row_characters.append(
                str(number) if number != EMPTY else EMPTY_CELL_DISPLAY_CHAR
            )
            len_row_characters = len(row_characters)
        if row_num % N_SQRT == 0 and row_num != 0:
            print("-" * (2 * len_row_characters - 1))
        print(" ".join(row_characters))
    print()
    print("+" * (2 * len_row_characters - 1))
    print()


def read_board_from_file(filename: str) -> Board:
    return io.read(filename=Path(filename), empty_number=EMPTY)


def generate_empty_board() -> Board:
    return [[EMPTY] * N for _ in range(N)]


def get_board(filename: str | None) -> Board:
    if filename:
        return read_board_from_file(filename=filename)
    return generate_empty_board()


def solve(board: Board) -> None:
    try_to_place(board, 0, 0)


if __name__ == "__main__":
    board = get_board(filename="../input_boards/004_5.csv")
    display_board(board=board)
    solve(board)

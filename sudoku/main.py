import heapq
from itertools import product
from pathlib import Path
from typing import Iterable

from pydantic import BaseModel, computed_field

from sudoku import io

NUM_ROWS = 9
NUM_COLS = 9

EMPTY_CELL_NUMBER = -1
EMPTY_CELL_DISPLAY_CHAR = "."

BOARD_ITERATOR = list(product(range(NUM_ROWS), range(NUM_COLS)))


class CellIndex(BaseModel):
    # Frozen (-ish)
    row: int
    col: int
    _impact_cells: list["CellIndex"] = []

    # Mutable
    number: int = EMPTY_CELL_NUMBER
    valid_numbers_left: set[int] = set(range(1, 10))

    def __lt__(self, other: "CellIndex"):
        # return (self.row, self.col) < (other.row, other.col)
        return len(self.valid_numbers_left) < len(other.valid_numbers_left)

    def __hash__(self):
        return hash((self.row, self.col))

    @property
    def is_empty(self) -> bool:
        return self.number == EMPTY_CELL_NUMBER

    # @computed_field
    # @property
    def impact_cells(self) -> list["CellIndex"]:
        if not self._impact_cells:
            cells = []
            cells.extend(self._get_cells_same_horizontal())
            cells.extend(self._get_cells_same_vertical())
            cells.extend(self._get_cells_same_bloc())
            self._impact_cells = list(set(cells))
        return self._impact_cells

    def display_cell_and_impact_cells(self) -> None:
        board = [[" " for _ in range(NUM_COLS)] for _ in range(NUM_ROWS)]
        board[self.row][self.col] = "x"
        for cell in self.impact_cells():
            board[cell.row][cell.col] = "o"
        for row in board:
            print(" ".join(str(x) for x in row))

    def _get_cells_same_horizontal(self) -> list["CellIndex"]:
        cells = []
        for d_col in range(1, 9):
            col = (self.col + d_col) % 9
            cells.append(CellIndex(row=self.row, col=col))
        return cells

    def _get_cells_same_vertical(self) -> list["CellIndex"]:
        cells = []
        for d_row in range(1, 9):
            row = (self.row + d_row) % 9
            cells.append(CellIndex(row=row, col=self.col))
        return cells

    def _get_cells_same_bloc(self) -> list["CellIndex"]:
        def get_top_left_cell() -> "CellIndex":
            row = 3 * (self.row // 3)
            col = 3 * (self.col // 3)
            return CellIndex(row=row, col=col)

        top_left_cell = get_top_left_cell()
        cells = []
        for d_row in range(3):
            for d_col in range(3):
                row = top_left_cell.row + d_row
                col = top_left_cell.col + d_col
                cell = CellIndex(row=row, col=col)
                if cell != self:
                    cells.append(cell)
        return cells


class Board(BaseModel):
    board: list[list[CellIndex]] = [
        [CellIndex(row=row, col=col) for col in range(NUM_COLS)]
        for row in range(NUM_ROWS)
    ]
    # fewest_valid_numbers: list[tuple[int, CellIndex]] = []
    fewest_valid_numbers: list[tuple[CellIndex, int, int, set[int]]] = []

    def init_from_2d_array(self, board: list[list[int]]) -> None:
        initial_numbers = []
        for row, col in BOARD_ITERATOR:
            number = board[row][col]
            if number != EMPTY_CELL_NUMBER:
                initial_numbers.append((row, col, board[row][col]))
        self.initialize(initial_numbers=initial_numbers)

    def initialize(self, initial_numbers: Iterable[tuple[int, int, int]]) -> None:
        heapq.heapify(self.fewest_valid_numbers)
        for row, col, number in initial_numbers:
            self.place_number(row=row, col=col, number=number)
        for row, col in BOARD_ITERATOR:
            cell = self.board[row][col]
            if cell.is_empty:
                print(f"ROW: {row}, COL: {col}")
                print(f"VALID_NUMBERS_LEFT: {cell.valid_numbers_left}")
                heapq.heappush(
                    # self.fewest_valid_numbers, (len(cell.valid_numbers_left), cell)
                    self.fewest_valid_numbers,
                    (
                        cell,
                        cell.row,
                        cell.col,
                        cell.valid_numbers_left,
                    ),
                )
                print(f"HEAP: {self.fewest_valid_numbers}")
        print(f"INIT: {len(self.fewest_valid_numbers)}")

    def place_next_number(self) -> tuple[int, int, int]:
        row, col, number = self._find_next_number_to_place()
        self.place_number(row=row, col=col, number=number)
        return row, col, number

    def _find_next_number_to_place(self) -> tuple[int, int, int]:
        # _, cell = heapq.heappop(self.fewest_valid_numbers)
        _, row, col, valid_numbers_left = heapq.heappop(self.fewest_valid_numbers)
        while not valid_numbers_left:
            print(f"CELL: {row}, {col}, {valid_numbers_left}")
            # _, cell = heapq.heappop(self.fewest_valid_numbers)
            _, row, col, valid_numbers_left = heapq.heappop(self.fewest_valid_numbers)
        return row, col, valid_numbers_left.pop()

    def place_number(self, row: int, col: int, number: int) -> None:
        self.board[row][col].number = number
        self.board[row][col].valid_numbers_left = set()
        self._update_impacted_cells(row=row, col=col, number=number)

    def _update_impacted_cells(self, row: int, col: int, number: int) -> None:
        """
        Removes the _added_ number from the valid numbers left for each
        impacted cell.
        """
        # print(f"UPDATE: ROW: {row}, COL: {col}")
        for impact_cell in self.board[row][col].impact_cells():
            # print(
            #     f"PRE DISCARD: {self.board[impact_cell.row][impact_cell.col].valid_numbers_left}, DISCARDING: {number}, ROW: {impact_cell.row}, COL: {impact_cell.col}"
            # )
            # print(f"HEAP: {self.fewest_valid_numbers}")
            self.board[impact_cell.row][impact_cell.col].valid_numbers_left.discard(
                number
            )

    def display(self, last_cell_added: tuple[int, int] | None = None) -> None:
        for row_num, row in enumerate(self.board):
            row_characters = []
            for col_num, cell in enumerate(row):
                if col_num % 3 == 0 and col_num != 0:
                    row_characters.append("|")

                if last_cell_added and last_cell_added == (row_num, col_num):
                    number = f"\033[91m{cell.number}\033[0m"
                else:
                    number = str(cell.number)

                row_characters.append(
                    number
                    if cell.number != EMPTY_CELL_NUMBER
                    else EMPTY_CELL_DISPLAY_CHAR
                )
            if row_num % 3 == 0 and row_num != 0:
                print("-" * (2 * len(row_characters) - 1))
            print(" ".join(row_characters))


def solve():
    pass


if __name__ == "__main__":
    board_001 = io.read(filename=Path("inputs/002_4.csv"))

    board = Board()
    board.init_from_2d_array(board=board_001)
    # board.initialize(
    #     (
    #         (0, 0, 1),
    #         (6, 6, 2),
    #         (7, 4, 3),
    #         (8, 8, 4),
    #     )
    # )
    board.display()
    for _ in range(50):
        row, col, number = board.place_next_number()
        print(row, col, number)
        board.display(last_cell_added=(row, col))
        print(len(board.fewest_valid_numbers))

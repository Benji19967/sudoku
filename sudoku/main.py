import heapq
from itertools import product

from pydantic import BaseModel, computed_field

NUM_ROWS = 9
NUM_COLS = 9

EMPTY_CELL_NUMBER = -1
EMPTY_CELL_DISPLAY_CHAR = "."

BOARD_ITERATOR = product(range(NUM_ROWS), range(NUM_COLS))


class CellIndex(BaseModel):
    # Frozen (-ish)
    row: int
    col: int
    _impact_cells: list["CellIndex"] = []

    # Mutable
    number: int = EMPTY_CELL_NUMBER
    valid_numbers_left: set[int] = set(range(1, 10))

    def __lt__(self, other: "CellIndex"):
        return (self.row, self.col) < (other.row, other.col)

    def __hash__(self):
        return hash((self.row, self.col))

    @property
    def is_empty(self) -> bool:
        return self.number == EMPTY_CELL_NUMBER

    @computed_field
    @property
    def impact_cells(self) -> list["CellIndex"]:
        if not self._impact_cells:
            cells = []
            cells.extend(self._get_cells_same_horizontal())
            cells.extend(self._get_cells_same_vertical())
            cells.extend(self._get_cells_same_bloc())
            self._impact_cells = sorted(set(cells))
        return self._impact_cells

    def display_cell_and_impact_cells(self) -> None:
        board = [[" " for _ in range(NUM_COLS)] for _ in range(NUM_ROWS)]
        board[self.row][self.col] = "x"
        for cell in self.impact_cells:
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
    fewest_valid_numbers: list[tuple[int, CellIndex]] = []

    def initialize(self, initial_numbers: tuple[tuple[int, int, int], ...]) -> None:
        heapq.heapify(self.fewest_valid_numbers)
        for row, col, number in initial_numbers:
            self.place_number(row=row, col=col, number=number)
        for row, col in BOARD_ITERATOR:
            print(f"ROW: {row}, COL: {col}")
            cell = self.board[row][col]
            if cell.is_empty:
                heapq.heappush(
                    self.fewest_valid_numbers, (len(cell.valid_numbers_left), cell)
                )
        print(f"INIT: {len(self.fewest_valid_numbers)}")

    def place_next_number(self) -> tuple[int, int, int]:
        row, col, number = self._find_next_number_to_place()
        self.place_number(row=row, col=col, number=number)
        return row, col, number

    def _find_next_number_to_place(self) -> tuple[int, int, int]:
        _, cell = heapq.heappop(self.fewest_valid_numbers)
        return cell.row, cell.col, cell.valid_numbers_left.pop()

    def place_number(self, row: int, col: int, number: int) -> None:
        self.board[row][col].number = number
        self.board[row][col].valid_numbers_left = set()
        self._update_impacted_cells(row=row, col=col, number=number)

    def _update_impacted_cells(self, row: int, col: int, number: int) -> None:
        """
        Removes the _added_ number from the valid numbers left for each
        impacted cell.
        """
        for impact_cell in self.board[row][col].impact_cells:
            self.board[impact_cell.row][impact_cell.col].valid_numbers_left.discard(
                number
            )

    def display(self) -> None:
        for row_num, row in enumerate(self.board):
            row_characters = []
            for col_num, cell in enumerate(row):
                if col_num % 3 == 0 and col_num != 0:
                    row_characters.append("|")
                row_characters.append(
                    str(cell.number)
                    if cell.number != EMPTY_CELL_NUMBER
                    else EMPTY_CELL_DISPLAY_CHAR
                )
            if row_num % 3 == 0 and row_num != 0:
                print("-" * (2 * len(row_characters) - 1))
            print(" ".join(row_characters))


def solve():
    pass


if __name__ == "__main__":
    board = Board()
    board.initialize(
        (
            (0, 0, 1),
            (6, 6, 2),
            (7, 4, 3),
            (8, 8, 4),
        )
    )
    board.display()
    for _ in range(30):
        print(board.place_next_number())
        board.display()
        print(len(board.fewest_valid_numbers))

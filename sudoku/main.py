from pydantic import BaseModel

NUM_ROWS = 9
NUM_COLS = 9

EMPTY_CELL = -1
EMPTY_CELL_DISPLAY_CHAR = "."


class Cell(BaseModel, frozen=True):
    row: int
    col: int
    # number: int | None = None

    def __lt__(self, other):
        return tuple(self.dict().values()) < tuple(other.dict().values())

    def get_cells_same_horizontal(self) -> list["Cell"]:
        cells = []
        for d_col in range(1, 9):
            col = (self.col + d_col) % 9
            cells.append(Cell(row=self.row, col=col))
        return cells

    def get_cells_same_vertical(self) -> list["Cell"]:
        cells = []
        for d_row in range(1, 9):
            row = (self.row + d_row) % 9
            cells.append(Cell(row=row, col=self.col))
        return cells

    def get_cells_same_bloc(self) -> list["Cell"]:
        def get_top_left_cell() -> "Cell":
            row = 3 * (self.row // 3)
            col = 3 * (self.col // 3)
            return Cell(row=row, col=col)

        top_left_cell = get_top_left_cell()
        cells = []
        for d_row in range(3):
            for d_col in range(3):
                row = top_left_cell.row + d_row
                col = top_left_cell.col + d_col
                cell = Cell(row=row, col=col)
                if cell != self:
                    cells.append(cell)
        return cells

    def get_impact_cells(self) -> list["Cell"]:
        cells = []
        cells.extend(self.get_cells_same_horizontal())
        cells.extend(self.get_cells_same_vertical())
        cells.extend(self.get_cells_same_bloc())
        return sorted(set(cells))

    def display_cell_and_impact_cells(self) -> None:
        board = [[" " for _ in range(NUM_COLS)] for _ in range(NUM_ROWS)]
        board[self.row][self.col] = "x"
        for cell in self.get_impact_cells():
            board[cell.row][cell.col] = "o"
        for row in board:
            print(" ".join(str(x) for x in row))


class Board(BaseModel):
    board: list[list[int]] = [
        [EMPTY_CELL for _ in range(NUM_COLS)] for _ in range(NUM_ROWS)
    ]

    def initialize(self, initial_numbers: tuple[tuple[int, int, int], ...]) -> None:
        for row, col, number in initial_numbers:
            self.board[row][col] = number

    def compute_valid_numbers_left(self, cell: Cell) -> list[int]:
        numbers_left = set(range(1, 10))
        impact_cells = cell.get_impact_cells()
        for cell in impact_cells:
            numbers_left.remove(Board.board[cell.row][cell.col])
        return list(numbers_left)

    def display(self) -> None:
        for row_num, row in enumerate(self.board):
            row_characters = []
            for col_num, number in enumerate(row):
                if col_num % 3 == 0 and col_num != 0:
                    row_characters.append("|")
                row_characters.append(
                    str(number) if number != EMPTY_CELL else EMPTY_CELL_DISPLAY_CHAR
                )
            if row_num % 3 == 0 and row_num != 0:
                print("-" * (2 * len(row_characters) - 1))
            print(" ".join(row_characters))

            # print(
            #     " ".join(
            #         str(number) if number != EMPTY_CELL else EMPTY_CELL_DISPLAY_CHAR
            #         for number in row
            #     )
            # )


def update_affecting_cells(cell: int) -> None:
    pass


def solve():
    cells = [list(range(NUM_COLS)) for _ in range(NUM_ROWS)]

    # board.compute_valid_numbers_left()


if __name__ == "__main__":
    # solve()
    # cell = Cell(row=8, col=8)
    # cell.display_cell_and_impact_cells()
    # print(len(cell.get_impact_cells()))

    board = Board()
    board.initialize(((0, 0, 1), (8, 8, 1)))
    board.display()

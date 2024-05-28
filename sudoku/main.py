from collections import defaultdict

from pydantic import BaseModel


# def compute_affected_cells(cells: list[int]) -> dict[int, list[int]]:
def compute_affected_cells(cells: list[list[int]]) -> dict[int, list[int]]:
    affected_cells = defaultdict(list)

    for i in range(9):
        for j in range(9):
            # horizontal
            for i in range(1, 9):
                affected_cell_id = cell_id + i
                affected_cells[cell_id].append()

            # vertical

            # bloc


def compute_valid_numbers_left(cell: int) -> list[int]:
    pass


def update_affecting_cells(cell: int) -> None:
    pass


def solve():
    # cells = list(range(1, 82))

    cells = [list(range(9)) for _ in range(9)]

    compute_affecting_cells(cells=cells)
    compute_valid_numbers_left()


if __name__ == "__main__":
    solve()

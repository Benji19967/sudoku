from pathlib import Path


def read(filename: Path) -> list[list[int]]:
    with open(filename) as f:
        board = []
        for line in f:
            board.append(
                [int(num) if num != "x" else -1 for num in line.strip().split(",")]
            )
    return board

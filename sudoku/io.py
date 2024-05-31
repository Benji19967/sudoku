from pathlib import Path


def read(filename: Path, empty_number: int = -1) -> list[list[int]]:
    with open(filename) as f:
        board = []
        for line in f:
            board.append(
                [
                    int(num) if num != "x" else empty_number
                    for num in line.strip().split(",")
                ]
            )
    return board

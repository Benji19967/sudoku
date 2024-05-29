# Sudoku solver

## First Algorithm


Pre-computation
- For each cell, compute the number of valid numbers left
- For each cell, compute other cells it affects (vertical, horizontal, bloc)

Loop
- Sort cells by count of valid numbers left, increasing
- Insert number into cell with lowest valid numbers left 
- Update affected cells

## Software Engineering

- What is the best way to represent the Board?
    - `list[list[cell]]` vs `dict[(row,coll), cell]`
- What is the best way to access a cell of the Board?
    - Is it best to just work with row/i, col/j?

# Sudoku solver

## First Algorithm


Pre-computation
- For each cell, compute the number of valid numbers left
- For each cell, compute other cells it affects (vertical, horizontal, bloc)

Loop
- Sort cells by count of valid numbers left, increasing
- Insert number into cell with lowest valid numbers left 
- Update affected cells

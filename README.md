# Sudoku

## Benchmarks

### C

```bash
hyperfine build/sudoku
```
```
Benchmark 1: build/sudoku
  Time (mean ± σ):       8.7 ms ±   0.2 ms    [User: 8.3 ms, System: 0.2 ms]
  Range (min … max):     8.3 ms …   9.6 ms    253 runs
```

### Python

```bash
hyperfine python sudoku/backtracking.py
```
```
Benchmark 1: python
  Time (mean ± σ):      14.6 ms ±   0.4 ms    [User: 10.9 ms, System: 2.8 ms]
  Range (min … max):    13.9 ms …  15.8 ms    170 runs
```

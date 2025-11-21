# Day 3 — Rule of 0/3/5/6 & Special Members

## Goals
- Understand when the compiler can generate special member functions for you.
- Contrast the Rule of Zero with explicit control under the Rule of 3/5/6.
- Implement a `Matrix` type with copy/move constructors and assignments.
- Measure the performance difference between copies and moves.

## Layout
| Path | Description |
| --- | --- |
| `include/matrix.hpp` | Header-only `Matrix` class with explicit special members and helper utilities. |
| `examples/special_members.cpp` | Tiny program that contrasts Rule of Zero vs. Rule of Three vs. Rule of Five/Six. |
| `examples/matrix_benchmark.cpp` | Benchmarks copy vs. move construction of `Matrix` using `std::chrono`. |
| `docs/special_member_reference.md` | Explains the rules, when to rely on the compiler, and practical guidance. |

## Building
```bash
mkdir -p day3/build
c++ -std=c++20 -Iday3/include day3/examples/special_members.cpp -o day3/build/special_members -O0
c++ -std=c++20 -Iday3/include day3/examples/matrix_benchmark.cpp -o day3/build/matrix_benchmark -O0
```

Re-run the benchmark with optimizations to see how much the optimizer can erase the cost of copies vs. moves:
```bash
c++ -std=c++20 -Iday3/include day3/examples/matrix_benchmark.cpp -o day3/build/matrix_benchmark_O2 -O2
```

## Try it
```bash
./day3/build/special_members
./day3/build/matrix_benchmark
```

The `matrix_benchmark` prints both timings and hints if the move path wins. Increase `dimension` or `iterations` inside the file to magnify the gap.

## Exercises
1. Flip `kVerbose` in `matrix.hpp` to `true` to trace each copy/move call and correlate with the benchmark output.
2. Remove the explicit special members from `Matrix` one-by-one and see which operations break—this highlights when the Rule of Zero suffices.
3. Change the `Matrix` storage to a raw pointer and implement the Rule of Five manually; compare how much boilerplate you write versus the vector-based design.

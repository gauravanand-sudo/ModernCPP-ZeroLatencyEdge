# Day 1 — Compilation Pipeline & Translation Units

This mini-project demonstrates how C++ source files move through the toolchain on their way to becoming an executable.

## Files

- `include/message.hpp` — declaration of the helper function.
- `src/message.cpp` — definition of the helper function.
- `src/main.cpp` — entry point that calls the helper.

## Commands

Run each stage from inside the repository root:

```bash
# Preprocess (produces preprocessed source on stdout)
g++ -Iday01/include -E day01/src/main.cpp -o day01/build/main.ii

g++ -Iday01/include -E day01/src/message.cpp -o day01/build/message.ii

# Compile (generates assembly)
g++ -Iday01/include -S day01/src/main.cpp -o day01/build/main.s
g++ -Iday01/include -S day01/src/message.cpp -o day01/build/message.s

# Assemble (generates object files)
g++ -Iday01/include -c day01/src/main.cpp -o day01/build/main.o
g++ -Iday01/include -c day01/src/message.cpp -o day01/build/message.o

# Link (combines objects into an executable)
g++ day01/build/main.o day01/build/message.o -o day01/build/day1
```

For a visual overview of the toolchain, see [`pipeline.md`](pipeline.md).

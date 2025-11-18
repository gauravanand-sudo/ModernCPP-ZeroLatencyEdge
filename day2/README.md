# Day 2 — Value Categories & Return Value Optimization

## Goals
- Distinguish among **prvalue**, **xvalue**, and **lvalue** expressions.
- Observe how compilers optimize return-by-value through **(N)RVO**.
- Use the `Trace` class to log constructor, move, and destructor events.

## Layout
| Path | Description |
| --- | --- |
| `include/trace.hpp` | Header-only helper that logs every special member function invocation. |
| `examples/value_categories.cpp` | Working sample that overloads on `Trace&`, `const Trace&`, and `Trace&&` to surface value categories. |
| `examples/rvo.cpp` | Demonstrates guaranteed copy elision vs. conditional returns that typically require a move. |
| `build/` | Destination for compiled binaries / intermediates (ignored by git). |
| `docs/value_category_reference.md` | Deep-dive guide that explains every concept related to lvalues, rvalues, `&&`, moves, casts, and RVO. |

## Building
```
mkdir -p day2/build
# Value category probes
c++ -std=c++20 -Iday2/include day2/examples/value_categories.cpp -o day2/build/value_categories -O0
# RVO / move tracing
c++ -std=c++20 -Iday2/include day2/examples/rvo.cpp -o day2/build/rvo -O0
```

Re-run the RVO sample with different options to see optimizer behavior:
```
# Observe every move/copy
c++ -std=c++20 -fno-elide-constructors -Iday2/include day2/examples/rvo.cpp -o day2/build/rvo_no_elide -O0

# Compare optimizer decisions
c++ -std=c++20 -Iday2/include day2/examples/rvo.cpp -o day2/build/rvo_O2 -O2
```
Inspect the emitted assembly to see how `-O0` vs. `-O2` reshuffles moves:
```
objdump -d day2/build/rvo > day2/build/rvo_O0.asm
objdump -d day2/build/rvo_O2 > day2/build/rvo_O2.asm
```

## Exercises
1. **Identify value categories**: Run `day2/build/value_categories` and map each call site to the overload that fires.
2. **RVO prediction**: Compile `rvo.cpp` with `-fno-elide-constructors` and count how many move constructors run for `make_elidable` vs. `make_conditional`.
3. **Optimizer impact**: Build `rvo.cpp` with `-O0` and `-O2`, then diff the generated assembly to see when moves disappear.

## Deep Dive Reference
Consult [`docs/value_category_reference.md`](docs/value_category_reference.md) for:

- A matrix of value categories (glvalue, lvalue, xvalue, prvalue, rvalue) and when they appear.
- Explanations of `T&`, `const T&`, `T&&`, forwarding references, and member qualifiers.
- Guidance on when to pass lvalues vs. rvalues, how `std::move`/`std::forward` work, and how casts influence value categories.
- A detailed treatment of RVO/NRVO and how to observe copy elision with the provided samples.

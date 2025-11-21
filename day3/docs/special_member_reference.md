# Special Member Rules (0/3/5/6)

C++ automatically generates six *special member functions* for most classes:

- Default constructor
- Destructor
- Copy constructor
- Copy assignment operator
- Move constructor
- Move assignment operator

## Rule of Zero
Prefer to **own resources with RAII members** (`std::string`, `std::vector`, `std::unique_ptr`). When every data member already manages itself, the compiler-generated special members are correct and exception-safe. See `LogMessage` in `examples/special_members.cpp`.

## Rule of Three
If you manually declare a destructor, you almost certainly need a copy constructor and copy assignment operator to preserve invariants and prevent double frees. Before C++11 this captured the core guidance. `Buffer3` shows why: the destructor frees a raw pointer, so copy operations must allocate and deep copy.

## Rule of Five/Six
Move operations join the story in C++11. If your type manages a resource and you define *any* special member, provide all of:

1. Destructor
2. Copy constructor
3. Copy assignment
4. Move constructor
5. Move assignment

The sixth slot—default constructor—is optional but often present. Mark move operations `noexcept` when transferring ownership cannot throw; this unlocks optimizations in standard containers.

## Rule of Zero Applied to Moves
Often you can lean on the compiler even with move semantics. `Buffer5` in `examples/special_members.cpp` stores a `std::unique_ptr`, so move operations can be defaulted safely. Copy support requires a small manual shim (deep copy into a fresh `unique_ptr`), which demonstrates how Rule of Five code shrinks once a resource uses RAII.

## Practical Checklist
- Store resources in RAII wrappers whenever possible. This lets you embrace the Rule of Zero.
- If you must own a raw handle, implement all five/six operations explicitly.
- Prefer copy-and-swap or `std::unique_ptr` to keep copy assignment exception safe.
- Add `noexcept` to move operations so vectors and other containers can use moves in place of copies.
- Instrument your types (as in `matrix.hpp` with `kVerbose`) to watch when copies vs. moves happen.

# Day 2 Reference — Value Categories, References, and RVO

This guide expands on the Day 2 hands-on samples by explaining every concept that
shows up in value category discussions: lvalues vs. rvalues, expression
classifications, reference types (including `&&`), casting helpers, move
semantics, and how Return Value Optimization (RVO) keeps temporary objects from
materializing.

Use it together with the runnable programs in `day2/examples/` so each idea has a
concrete log you can observe via the `Trace` helper.

---

## Value Category Matrix

| Category | Alias | Definition | Typical Producers | Notes |
| --- | --- | --- | --- | --- |
| **glvalue** | general lvalue | An expression whose value refers to a memory location. | Named variables, dereferenced pointers, `std::get<0>(tuple)` | glvalues encompass both lvalues and xvalues. |
| **lvalue** | locator value | A glvalue that is not an xvalue. | Variables, function lvalue references, string literals. | Can bind to `T&` or `const T&`. Cannot bind to `T&&` without a cast. |
| **xvalue** | expiring value | A glvalue whose resources can be reused. | `std::move(obj)`, member access on `std::move(obj).field`, function return of `T&&`. | Binds to `T&&` and `const T&`. |
| **prvalue** | pure rvalue | Temporary values that do not refer to storage. | Literal `42`, `Foo{}`, arithmetic results, `std::string("hi")`. | Cannot bind to `T&`, but can initialize temporaries or bind to `const T&`. |
| **rvalue** | prvalue or xvalue | Expression you can treat as a source for moves. | `std::move(obj)`, temporaries, return-by-value results. | Rvalues bind to `T&&`/`const T&`. |

Key reminders:
- Every expression is exactly one of `{lvalue, xvalue, prvalue}`.
- glvalue = lvalue ∪ xvalue; rvalue = prvalue ∪ xvalue.

---

## Where Each Category Appears

- **lvalues** dominate when you name existing storage: assignment targets, objects
  that live past the current full-expression, or anything you can take the address
  of without a cast.
- **prvalues** typically appear when you construct new temporaries (`Trace{}`)
  or compute expressions (`a + b`).
- **xvalues** arise when you explicitly mark an object for reuse via
  `std::move(obj)`, call a function that returns `T&&`, or access a member of an
  rvalue (`std::move(vec).size()`).

Run `day2/examples/value_categories.cpp` to see how overload resolution reacts to
each case.

---

## Reference Types and `&&`

### Lvalue references (`T&`)
- Bind only to lvalues (or lvalue subobjects).
- Preserve identity: modifications through the reference change the original.

### Const lvalue references (`const T&`)
- Bind to lvalues **and** rvalues by creating a temporary when needed.
- Useful for accepting read-only arguments regardless of value category.

### Rvalue references (`T&&`)
- Bind to rvalues (prvalues or xvalues) but not plain lvalues.
- Indicate ownership transfer opportunities; enable move constructors and move
  assignment operators.

### Forwarding references (`T&&` in a template with type deduction)
- Also called *universal references*.
- Reference collapsing rules determine the final type:
  - `T& &` → `T&`
  - `T& &&` → `T&`
  - `T&& &` → `T&`
  - `T&& &&` → `T&&`
- `std::forward<T>(param)` preserves the caller's value category.

### Member function qualifiers
- Methods can be overloaded with `&`, `&&`, `const&`, `const&&` to express how
  the implicit `*this` participates in overload resolution.

---

## Move Semantics and `std::move`

The `Trace` helper logs when move constructors and move assignments run. Observe
these events in both example programs.

- `std::move(obj)` is a cast to `T&&`; it **does not** move anything itself.
- Moving is implemented by constructors/assignments that accept `T&&`.
- After moving, the source object remains in a valid but unspecified state.
- Prefer `std::move_if_noexcept` in containers to avoid moving if it may throw.

Example snippet (see `examples/rvo.cpp`):
```cpp
Trace make_trace() {
    Trace local{"local"};
    return local; // Mandatory NRVO in C++17, move otherwise.
}
```
- With NRVO, no move occurs; the compiler constructs the result directly in the
  caller's storage.
- Disable elision with `-fno-elide-constructors` to force the move constructor.

---

## When Do You Use lvalues vs. rvalues?

| Task | Preferred Category | Reason |
| --- | --- | --- |
| Mutating existing objects | lvalue (`T&`) | Need identity and stable address. |
| Consuming temporaries | rvalue (`T&&`) | Can steal resources; no aliasing. |
| Function parameters that only read | `const T&` | Accept both categories without copies. |
| Storing in containers | prvalue | Emplace constructs in-place from temporaries. |
| Rebinding smart pointers | xvalue | `std::move(ptr)` transfers ownership. |

---

## Casting and Value Categories

Casting changes both type and value category:

- **Implicit conversions**: `return Trace{}` yields a prvalue. Binding to `const Trace&`
  materializes a temporary with automatic storage.
- **`static_cast<T&&>`**: Equivalent to `std::move`; use when you need explicit
  control outside `<utility>`.
- **`const_cast`**: Can remove qualifiers but does **not** change value category.
- **C-style casts**: Try several cast forms; prefer the explicit C++ casts to avoid
  surprises.
- **Materialization**: prvalues become temporary objects when they need storage
  (e.g., binding to `const T&` or passing to `T&&`).

### Example
```cpp
Trace source{"src"};
Trace&& sink = static_cast<Trace&&>(source); // xvalue via cast
```
Without the cast, `source` is an lvalue and would not bind to `Trace&&`.

---

## Return Value Optimization (RVO) and Copy Elision

### Mandatory elision (C++17 and later)
The compiler must omit copies/moves when:
1. A function returns the same named automatic object (`return local;`).
2. A function returns a braced initializer (`return Trace{"temp"};`).
3. A `try` block with no `catch` is returning the operand of a `co_return` or
   `throw` expression (for completeness).

### Named RVO (NRVO)
When multiple return statements can select different named objects, elision is not
mandatory. Compilers still usually elide at `-O2`. Compile `rvo.cpp` with
`-fno-elide-constructors` to reveal fallback moves.

### Observing the effect
- Build `examples/rvo.cpp` at `-O0` and `-O2` and compare the logs.
- Inspect assembly to see whether constructors are inlined or skipped entirely.

---

## Putting It All Together

1. Run `value_categories.cpp` to correlate overload selection with value
   categories.
2. Run `rvo.cpp` at different optimization levels to observe when RVO/moves kick
   in.
3. Experiment with your own functions: add overloads for `Trace&`, `const Trace&`,
   and `Trace&&` in a scratch file, then forward arguments with `std::move` or
   `std::forward` to predict the logs before you compile.

Because the `Trace` helper logs every special member, you can attach it to any new
experiment and immediately see how lvalues, rvalues, and references influence the
object lifetime.

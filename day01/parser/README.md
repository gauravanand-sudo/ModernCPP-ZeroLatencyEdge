# Day 1 Parser Mini-Project

This folder contains a tiny arithmetic expression parser built with Lex (Flex) and Yacc (Bison).
It complements the compilation-pipeline exercises by showing another pair of translation
units (the generated scanner and parser) working together.

## Requirements

- `flex` (or another Lex-compatible scanner generator)
- `bison` (or another Yacc-compatible parser generator)
- A C++17 compiler such as `g++`

## Build & Run

```bash
cd day01/parser
make         # generates parser.tab.c/.h and lex.yy.c, then builds the `calc` binary
./calc <<'INPUT'
2 + 3 * 4
(1 + 2) * 5
INPUT
```

Each line is parsed independently, and the calculator echoes the integer result prefixed
with `=`.

## How It Works

- `parser.y` defines a grammar for integer expressions plus the semantic actions that
  compute the result.
- `lexer.l` tokenizes numbers, operators, parentheses, and newlines, feeding tokens into
  the parser.
- `Makefile` wires everything together so you can regenerate the parser quickly.

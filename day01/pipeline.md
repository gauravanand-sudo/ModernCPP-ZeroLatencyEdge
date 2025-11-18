# Day 1 — Compilation Pipeline

```
source code (.cpp/.hpp)
        |
        v
   Preprocess (g++ -E)
        |
        v
  Preprocessed source (.ii)
        |
        v
     Compile (g++ -S)
        |
        v
   Assembly output (.s)
        |
        v
    Assemble (g++ -c)
        |
        v
  Object code (.o)
        |
        v
      Linker (g++)
        |
        v
     Executable (a.out)
```

Use the commands below to walk through each stage manually.

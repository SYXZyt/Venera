# Venera
Official compiler of the Venera hobby language.
[Official website](https://veneralang.com/)

> **Notice:** The compiler is in very early development. Currently, only a function and a return can be compiled. Check the `dev` branch for the most up to date compiler.


## Example
This is the current limit of what the compiler supports.

```
PROGRAM DO
    DEF I32 Main DO
        RETURN 14
    END
END
```

Compiling this produces a native executable that exits with code `14`.

## Building

Requirements:

- CMake 3.20+
- A C++23 compiler
- LLVM (found via `find_package(LLVM CONFIG)`)
- `clang` on `PATH` (used as the linker)

```sh
cmake -S . -B build -G Ninja
cmake --build build
```

## Usage

The compiler doesn't take command-line arguments yet. The source program is hardcoded in [venera/main.cpp](venera/main.cpp). Running `venera` prints the token stream and AST, then writes `prog.o` and links it into `prog` in the current directory.

Exit codes: `1` lexing failed, `2` parsing failed, `3` analysis failed, `4` code generation or linking failed.

## Error codes

| Range   | Stage                     |
|---------|---------------------------|
| `E0xxx` | General                   |
| `E1xxx` | Lexing                    |
| `E2xxx` | Parsing                   |
| `E3xxx` | Analysis                  |
| `W3xxx` | Analysis warnings         |

After adding a code to `ErrorCode.h`, run `python3 venera/scripts/generate_error_codes.py` to update `ErrorCode.cpp`.
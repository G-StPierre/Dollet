# DOLLET - A C-Style Interpreted Programming Language

Dollet is a lightweight interpreted programming language written from scratch in C (C17 standard). The language was designed to explore and get a greater understanding of the various components that make up a programming language such as lexing, parsing, interpreting, etc.

## Features

- **C-Style syntax**
    Easily understandable for programmers coming from c-style languages.

- **Fast Interpreter written in C**
    Written completely using the C standard library, no unneeded external dependencies.

- **Small and Minimal Standard Library**
    Small standard library allowing access to the `print()` function to output directly to stdout.

## Reason for Creation

The main goal of this language was to gain a deep understanding of how programming languages work under the hood. By building Dollet, I learned about:

- **Lexers** — Specifically single-layer FSM lexers that read input and matched them to a known set of tokens.
- **Recursive Descent Parsers** — To build abstract syntax trees (ASTs).
- **Abstract Syntax Trees** — To allow the interpreter to process expressions cleanly and deterministically.
- **Interpreters** — Recursively evaluating expressions from the AST.


## Example Code

```c
    var t = 5;
    fun square(x) {return x * x;}
    var result = square(t);
    print(result);
```
## Building

### Requirements
    - A C(17) Compiler (GCC, Clang, etc)
    - make (usually preinstalled on Linux/macOS)
        - Windows 
            You can use make via MinGW, MSYS2, or Windows Subsystem for Linux (WSL). Alternatively, you can compile manually using gcc.

### Build
```bash
    git clone https://github.com/G-StPierre/Dollet.git
    cd dollet
    make
```
### Running
```bash
    cd build
    ./dollet <source-code-file>
```
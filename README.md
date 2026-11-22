# C Preprocessor Analyzer

## Project Overview
The C Preprocessor Analyzer is a software-based project developed using C. It demonstrates the basic working stages of a C preprocessor by analyzing and processing C source files.

## Features

The project performs four main stages:

1. **Header Inclusion** – Includes the contents of user-defined header files.
2. **Comment Removal** – Removes single-line and multi-line comments.
3. **Conditional Compilation** – Processes conditional directives such as `#ifdef`, `#ifndef`, `#else`, and `#endif`.
4. **Macro Replacement** – Replaces defined macros with their corresponding values.

## Technologies Used
- C Programming
- GCC Compiler
- VS Code

## Project Structure


C_Preprocessor_Analyzer/
├── input/
│   ├── main.c
│   └── myheader.h
├── output/
├── main.c
├── analyzer.c
├── .gitignore
└── README.md
```

## How to Run

Compile the project:


gcc main.c analyzer.c -o analyzer

Run the program on Windows:


.\analyzer.exe


## Output
The program generates four intermediate files:

- `temp.c`
- `cleaned.c`
- `conditional.c`
- `processed.c`

## Note
This is an educational implementation demonstrating basic preprocessor operations. It is not a complete replacement for the standard C preprocessor.

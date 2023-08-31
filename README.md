# Loo Custom Programming Language

Compiler built using C++, compiles to x86_64 assembly code in Ubuntu (WSL).

## Running the Loo Compiler

Compile the complier via cmake: `cmake --build build/`<br>
Run the complier given an \<input.loo> file: `./build/loo <input.loo>`

## Loo Syntax

Exiting Program: `return <int code>` to exit the program with code \<int code>
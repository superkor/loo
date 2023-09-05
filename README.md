# Loo Custom Programming Language

Compiler built using C++, compiles to x86_64 assembly code in Ubuntu (WSL).

## Running the Loo Compiler

Compile the complier via cmake: `cmake --build build/`<br>
Run the complier given an \<input.loo> file: `./build/loo <input.loo>`

### Output Files

Running the complier will output a `out.asm` (linux x86_64 assembly) file, a `out.o` object file, and a `out` executable file.

## Loo Syntax

Exiting Program: `exit(<int code>);` to exit the program with code \<int code>.

Example: `exit(100);` will exit the program with exit code 100.
# Brainfuck Compiler (WIP)
A brainfuck compiler written in C11

I made this project as a quick showcase of my C programming
style, as I think C let's me express myself as a programmer
more than any other language.

### Build
Simply use `make` for standard release build, `make debug` to
generate debug information throughout the compilation process
or `make test` to generate a test build (you should use this
when you contribute).

**NOTE** Standard release and test builds currently have no
functionality.

### Usage
Currently the compiler only generates pseudo instructions with
only one strength-reduction optimization implemented, and that
is reducing `add` and `sub` instructions for increments, decrements
and shifts (both left and right).
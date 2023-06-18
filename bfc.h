#ifndef BFC_H
#define BFC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

// Get all valid instructions from a brainfuck source
// file specified by "path", then store said instructions
// into a buffer that is then returned. Returns NULL on
// failure. The `num_instructions` pointer has its data
// modified.
// 
// THIS METHOD ALLOCATES MEMORY TO THE BUFFER PARAMETER
// AND THIS MEMORY MUST BE FREED BY THE USER
char* get_file_contents(const char* path, size_t* num_instructions);

typedef union
{
    unsigned char value; // TODO maybe make larger
    size_t label;
} InstructionArg;

typedef struct
{
    char token;
    InstructionArg arg;
} Operation;

Operation* generate_pseudo_instructions(char* tokens, size_t* num_instructions);

#endif
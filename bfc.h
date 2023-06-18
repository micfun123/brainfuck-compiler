#ifndef BFC_H
#define BFC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

// Get all valid instructions from the file specified by the 'path'
// parameter. Returns NULL on error. On success the value at the
// address specified by num_instructions will change to the number
// of instruction characters that were successfully read.
// 
// All invalid characters will be treated as comments.
// 
// THIS FUNCTION RETURNS A POINTER TO ALLOCATED MEMORY WHICH MUST BE FREED
// BY THE USER
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

// Returns a pointer to an array of operations (pseudo instructions)
// On error this will return NULL. On Success the value at the
// pointer specified by num_instructions will be changed to match
// the amount of pseudo instructions (it is important to consider this
// as optimizations performed in this function will likely always result in
// a reduction in the number of instructions).
// 
// THIS FUNCTION RETURNS A POINTER TO ALLOCATED MEMORY WHICH MUST BE FREED
// BY THE USER
Operation* generate_pseudo_instructions(char* tokens, size_t* num_instructions);

#endif
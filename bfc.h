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

typedef enum
{
    ADD,        // +
    SUB,        // -
    SHIFT_L,    // <
    SHIFT_R,    // >
    INPUT,      // ,
    OUTPUT,     // .
    OPEN_LOOP,  // [
    CLOSE_LOOP  // ]
} Token;

// Allocate, then return an array of tokens of size `size` based
// on the characters within `buffer` (which is also of size `size`)
// 
// Returns token array on success, null on failure.
Token* tokenize(const char* buffer, size_t size);

#endif
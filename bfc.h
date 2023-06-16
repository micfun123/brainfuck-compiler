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

// Tokens
// Why typedef + macros and not enum:
//   I would like each char's integer value to be assigned a name.
//   Why use an enum when I assigning every value anyways? At that
//   point it might might as well be a macro.
#define TOK_INC    '+'
#define TOK_DEC    '-'
#define TOK_LSHFT  '<'
#define TOK_RSHFT  '>'
#define TOK_IN     ','
#define TOK_OUT    '.'
#define TOK_OPEN   '['
#define TOK_CLOSE  ']'

#endif
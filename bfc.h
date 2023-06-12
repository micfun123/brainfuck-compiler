#ifndef BFC_H
#define BFC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Get all valid instructions from a brainfuck source
// file specified by "path", then store said instructions
// into specified "buffer". Returns the amount of valid
// instructions read or -1 on failure.
// 
// THIS METHOD ALLOCATES MEMORY TO THE BUFFER PARAMETER
// AND THIS MEMORY MUST BE FREED BY THE USER
int get_file_contents(const char* path, char** buffer);

#endif
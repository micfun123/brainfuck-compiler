#include "bfc.h"

// TODO test on clang
// TODO test on TCC
#ifndef __GNUC__
#warning "This software has only been built and tested with GCC."
#endif

// TODO test on windows with mingw
#ifndef __unix__
#warning "This software has only been tested on a Unix system."
#endif

// TODO add ARM support
#if !defined(__x86_64__) && !defined(__i386__)
#warning "This software has only been tested when compiled to x86."
#endif

// TODO maybe clean these up
#if defined(CCHECK)
    #undef CCHECK
#endif
#define CCHECK(exp)                         CCHECK_PRNT_CLEANUP(exp, fprintf(stderr,"%s:%d failed assertion '%s'\n",__FILE__,__LINE__,#exp), exit(1))
#define CCHECK_CLEANUP(exp,cleanup)         CCHECK_PRNT_CLEANUP(exp, fprintf(stderr,"%s:%d failed assertion '%s'\n",__FILE__,__LINE__,#exp), cleanup)
#define CCHECK_MSG(exp,msg)                 CCHECK_PRNT_CLEANUP(exp, fprintf(stderr,"%s\n",msg),                                             exit(1))
#define CCHECK_MSG_CLEANUP(exp,msg,cleanup) CCHECK_PRNT_CLEANUP(exp, fprintf(stderr,"%s\n",msg),                                             cleanup)
#define CCHECK_PRNT(exp,prnt)               CCHECK_PRNT_CLEANUP(exp, prnt,                                                                   exit(1))
#define CCHECK_PRNT_CLEANUP(exp,prnt,cleanup) \
 do { \
     if( !(exp) ) { \
         prnt; \
         cleanup; \
     } \
 } while(0)

#define NUM_VALID_CHARS 8
const char valid_characters[NUM_VALID_CHARS] = "<>+-[].,";

int main(int argc, char* argv[])
{
    char* buffer = NULL;

    CCHECK_MSG(argc == 2, "Usage: bfc <INPUT_FILE>");

    size_t path_length = strlen(argv[1]);
    CCHECK_PRNT(path_length > 3, fprintf(stderr, "'%s' is not a brainfuck source file\n", argv[1]));
    CCHECK_PRNT(strcmp(argv[1] + (path_length - 3), ".bf") == 0, fprintf(stderr, "'%s' is not a brainfuck source file\n", argv[1]));

    int result = get_file_contents(argv[1], &buffer);
    CCHECK_PRNT_CLEANUP(result >= 0, fprintf(stderr, "Unable to open file '%s'\n", argv[1]), goto CLEANUP);

    #if defined(DEBUG)
        printf("Source: %s\n", buffer);
    #endif

    free(buffer);
    return 0;

CLEANUP:
    free(buffer);
    return 1;
}

int get_file_contents(const char* path, char** buffer)
{
    FILE* fp = NULL;

    fp = fopen(path, "r");
    if(fp == NULL)
    {
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    int num_characters = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    *buffer = calloc(num_characters + 1, 1);

    CCHECK_MSG_CLEANUP(buffer != NULL, "Failed calloc() when reading file contents", goto CLEANUP);

    int num_instructions = 0;
    char c;
    while( (c = fgetc(fp)) != EOF)
    {
        for(int i = 0; i < NUM_VALID_CHARS; i++)
        {
            if(c == valid_characters[i])
            {
                (*buffer)[num_instructions++] = c;
                break;
            }
        }
    }

    if(num_instructions != num_characters)
    {
        *buffer = realloc(*buffer, num_instructions + 1);
        CCHECK_MSG_CLEANUP(buffer != NULL, "Failed realloc() when reading file contents.", goto CLEANUP);
    }

    fclose(fp);
    return num_instructions;

CLEANUP:
    fclose(fp);
    return -1;
}
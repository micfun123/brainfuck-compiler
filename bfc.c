#include "bfc.h"

// TODO maybe clean these up
#if defined(CASSERT)
    #undef CASSERT
#endif
#define CASSERT(exp) do{if(!(exp)){fprintf(stderr,"%s:%d failed assert '%s'\n",__FILE__,__LINE__,#exp);exit(1);}}while(0)
#define CASSERT_CLEANUP(exp, cleanup) do{if(!(exp)){fprintf(stderr,"%s:%d failed assert '%s'\n",__FILE__,__LINE__,#exp);cleanup;}}while(0)
#define CASSERT_MSG(exp, msg) do{if(!(exp)){fprintf(stderr,"%s\n",msg);exit(1);}}while(0)
#define CASSERT_MSG_CLEANUP(exp, msg, cleanup) do{if(!(exp)){fprintf(stderr,"%s\n",msg);cleanup;}}while(0)
#define CASSERT_PRNT(exp, prnt) do{if(!(exp)){prnt;exit(1);}}while(0)
#define CASSERT_PRNT_CLEANUP(exp, prnt, cleanup) do{if(!(exp)){prnt;cleanup;}}while(0)

#define NUM_VALID_CHARS 8
const char valid_characters[NUM_VALID_CHARS] = "<>+-[].,";

int main(int argc, char* argv[])
{

    // All pointers to allocated memory are
    // declared and initialized here to keep
    // the code clean since not all will be
    // allocated when they need to be freed
    // i.e. on error
    char* buffer = NULL;

    CASSERT_MSG(argc == 2, "Usage: bfc <INPUT_FILE>");

    size_t path_length = strlen(argv[1]);
    CASSERT_PRNT(path_length > 3, fprintf(stderr, "'%s' is not a brainfuck source file\n", argv[1]));
    CASSERT_PRNT(strcmp(argv[1] + (path_length - 3), ".bf") == 0, fprintf(stderr, "'%s' is not a brainfuck source file\n", argv[1]));

    int result = get_file_contents(argv[1], &buffer);
    CASSERT_PRNT_CLEANUP(result >= 0, fprintf(stderr, "Unable to open file '%s'\n", argv[1]), goto CLEANUP);

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
    CASSERT_MSG_CLEANUP(buffer != NULL, "", goto CLEANUP);

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
        CASSERT_MSG_CLEANUP(buffer != NULL, "", goto CLEANUP);
    }

    fclose(fp);
    return num_instructions;

CLEANUP:
    fclose(fp);
    return -1;
}
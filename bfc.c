#include "bfc.h"

// TODO test on clang
// TODO test on TCC
#ifndef __GNUC__
#warning "This software has only been built and tested with GCC."
#endif

// TODO test on windows with mingw
#ifndef __unix__
#warning "This software has only been tested on a Unix-like system."
#endif

// TODO test for ARM
#if !defined(__x86_64__) && !defined(__i386__)
#warning "This software has only been tested when compiled to x86."
#endif

// TODO remove later
#if !defined(DEBUG) && !defined(TEST)
#warning "You are compiling a non-debug and non-test version of WIP software. Currently, it will do nothing. Please build under debug to see the output"
#endif

#if defined(CASSERT)
#undef CASSERT
#endif
#define CASSERT(exp)         CASSERT_PRNT(exp, fprintf(stderr,"%s:%d failed assertion '%s'\n",__FILE__,__LINE__,#exp))
#define CASSERT_MSG(exp,msg) CASSERT_PRNT(exp, fprintf(stderr,"%s\n",msg))
#define CASSERT_PRNT(exp,prnt) \
do { \
    if( !(exp) ) { \
        prnt; \
    } \
} while(0)

#if defined(CCHECK)
#undef CCHECK
#endif
#define CCHECK(exp)                         CCHECK_PRNT_CLEANUP(exp, fprintf(stderr,"%s:%d failed check '%s'\n",__FILE__,__LINE__,#exp), exit(EXIT_FAILURE))
#define CCHECK_CLEANUP(exp,cleanup)         CCHECK_PRNT_CLEANUP(exp, fprintf(stderr,"%s:%d failed check '%s'\n",__FILE__,__LINE__,#exp), cleanup)
#define CCHECK_MSG(exp,msg)                 CCHECK_PRNT_CLEANUP(exp, fprintf(stderr,"%s\n",msg),                                         exit(EXIT_FAILURE))
#define CCHECK_MSG_CLEANUP(exp,msg,cleanup) CCHECK_PRNT_CLEANUP(exp, fprintf(stderr,"%s\n",msg),                                         cleanup)
#define CCHECK_PRNT(exp,prnt)               CCHECK_PRNT_CLEANUP(exp, prnt,                                                               exit(EXIT_FAILURE))
#define CCHECK_PRNT_CLEANUP(exp,prnt,cleanup) \
do { \
    if( !(exp) ) { \
        prnt; \
        cleanup; \
    } \
} while(0)

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

#define NUM_VALID_CHARS 8
const char valid_characters[NUM_VALID_CHARS] = {TOK_INC, TOK_DEC, TOK_LSHFT, TOK_RSHFT, TOK_IN, TOK_OUT, TOK_OPEN, TOK_CLOSE};

#if defined(DEBUG)
const char* TOKEN_NAMES[] = {"ADD", "SUB", "LSHFT", "RSHFT", "IN", "OUT", "OPEN", "CLOSE"};
#endif

#define JMP_STACK_SIZE ( 1024 / sizeof(size_t) ) // We don't want more than a MB

// CODE STYLE
//   Each function has an integer variable called exit_code that is returned every time.
//   The exit code should have a default failure value. When things fail unexpectedly,
//   a CCHECK should be used to report the error and use a `goto` to skip any logic that
//   changes the exit_code variable from the default failure value.
// 
//   Non-fatal errors should be handelled with asserts. A failed assert should be reported,
//   but will not cause the entire program to abort.
// 
//   Variables should be declared close to where they are first used with the following
//   exceptions:
//     - Exit/Error codes
//     - Return values
//     - Allocated memory that needs/may need to be freed locally for any reason
//
//   Debug (BUT NOT TEST!) code must be contained within an `#if defined(DEBUG)` block
//   Test (BUT NOT DEBUG!) code must be contained within an `#if defined(TEST)` block

int main(int argc, char* argv[])
{
    int exit_code = EXIT_FAILURE;

    char* tokens = NULL;
    Operation* operations = NULL;

    CCHECK_MSG(argc == 2, "Usage: bfc <INPUT_FILE>");

    size_t path_length = strlen(argv[1]);
    CCHECK_PRNT(path_length > 3, fprintf(stderr, "'%s' is not a brainfuck source file\n", argv[1]));
    CCHECK_PRNT(strcmp(argv[1] + (path_length - 3), ".bf") == 0, fprintf(stderr, "'%s' is not a brainfuck source file\n", argv[1]));

    size_t num_instructions = 0;
    tokens = get_file_contents(argv[1], &num_instructions);
    CCHECK_PRNT_CLEANUP(tokens != NULL, fprintf(stderr, "Failed to grab file contents from '%s'\n", argv[1]), goto CLEANUP);

    #if defined(DEBUG)
        printf("Source: %s\n", tokens);
        printf("Tokens:\n");
        for(size_t i = 0; i < num_instructions; i++)
        {
            for(int j = 0; j < NUM_VALID_CHARS; j++)
            {
                if(tokens[i] == valid_characters[j])
                {
                    printf("  %s\n", TOKEN_NAMES[j]);
                }
            }
        }
    #endif

    operations = generate_pseudo_instructions(tokens, &num_instructions);
    CCHECK_MSG_CLEANUP(operations != NULL, "Failed to generate pseudo instructions", goto CLEANUP);

    #if defined(DEBUG)
        printf("Operations:\n");
        for(size_t i = 0; i < num_instructions; i++)
        {
            printf("%u  %c", i, operations[i].token);
            switch(operations[i].token)
            {
            case TOK_INC:
            case TOK_DEC:
            case TOK_LSHFT:
            case TOK_RSHFT:
                printf(": %u\n", operations[i].arg.value);
                break;
            case TOK_OPEN:
                printf(": Label number %u\n", operations[i].arg.label);
                break;
            case TOK_CLOSE:
                printf(": Jump to instruction %u\n", operations[i].arg.label);
                break;
            case TOK_IN:
            case TOK_OUT:
                printf("\n");
                break;
            default:
                // Should not get here
                CASSERT_MSG(false, "Invalid token somehow processed in `main()` during debug print.");
            }
        }
    #endif

    exit_code = EXIT_SUCCESS;
    CLEANUP:
    free(tokens);
    free(operations);
    return exit_code;
}

char* get_file_contents(const char* path, size_t* num_instructions)
{
    char* return_val = NULL;

    FILE* fp = NULL;

    fp = fopen(path, "r");
    CCHECK_PRNT_CLEANUP(fp != NULL, fprintf(stderr, "Failed to open '%s' when attempting to read file contents", __FILE__), goto ABORT);

    fseek(fp, 0, SEEK_END);
    size_t num_characters = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* buffer = calloc(num_characters + 1, 1);

    CCHECK_MSG_CLEANUP(buffer != NULL, "Failed calloc() when reading file contents", goto ABORT);

    *num_instructions = 0;
    char c;
    while( (c = fgetc(fp)) != EOF)
    {
        for(size_t i = 0; i < NUM_VALID_CHARS; i++)
        {
            if(c == valid_characters[i])
            {
                buffer[(*num_instructions)++] = c;
                break;
            }
        }
    }

    return_val = realloc(buffer, *num_instructions);
    CASSERT_MSG(return_val != NULL, "realloc failed in `get_file_contents()`");
    ABORT:
    fclose(fp);
    return return_val;
}

Operation* generate_pseudo_instructions(char* tokens, size_t* num_instructions)
{
    Operation* return_val = NULL;
    
    Operation* code = malloc(sizeof(Operation) * (*num_instructions));
    CCHECK_MSG_CLEANUP(code != NULL, "Failed malloc in optimize_tokens()", goto ABORT);
    size_t jmp_stack[JMP_STACK_SIZE] = {0};

    size_t instruction_idx = 0, jmp_stack_idx = 0, label_counter = 0;
    for(size_t i = 0; i < *num_instructions; i++)
    {
        code[instruction_idx].token = tokens[i];
        switch(tokens[i])
        {
        case TOK_INC:
        case TOK_DEC:
        case TOK_LSHFT:
        case TOK_RSHFT:
            code[instruction_idx].arg.value = 1;
            while(tokens[i + 1] == tokens[i])
            {
                code[instruction_idx].arg.value++;
                i++;
            }
            break;

        case TOK_OPEN:
            CCHECK_MSG_CLEANUP(jmp_stack_idx < JMP_STACK_SIZE, "There are more '[' tokens than there are ']'", goto ABORT);
            code[instruction_idx].arg.label = label_counter++;
            jmp_stack[jmp_stack_idx++] = instruction_idx + 1;
            break;
        case TOK_CLOSE:
            CCHECK_MSG_CLEANUP(jmp_stack_idx > 0, "There are more ']' tokens than there are '['", goto ABORT);
            code[instruction_idx].arg.label = jmp_stack[--jmp_stack_idx];
            break;

        case TOK_IN:
        case TOK_OUT:
            break;
        default:
            // Should not get here
            CCHECK_PRNT_CLEANUP(false, fprintf(stderr, "Invalid token '%c' somehow processed in `generate_pseudo_instructions()`.", tokens[i]), goto ABORT);
        }
        instruction_idx++;
    }

    return_val = realloc(code, sizeof(Operation) * instruction_idx);
    *num_instructions = instruction_idx;
    CASSERT_MSG(return_val != NULL, "Failed realloc in `generate_pseudo_instructions()`");
    ABORT:
    return return_val;
}
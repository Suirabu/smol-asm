#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "lexer.h"
#include "token.h"
#include "util.h"

int main(int argc, char** argv) {
    if(argc < 2) {
        REPORT_ERROR("No input file provided\n");
        return EXIT_FAILURE;
    }

    const char* source_path = argv[1];
    
    char* source = util_get_file_contents(source_path);
    if(!source) {
        return EXIT_FAILURE;
    }

    Lexer lexer = { .source = source };
    Token* tokens = lexer_collect_tokens(&lexer);
    if(!tokens) {
        free(source);
        return EXIT_FAILURE;
    }

    while(tokens->type != TOK_EOF) {
        Token token = *tokens++;

        if(token.type == TOK_LABEL) {
            printf("Line %-4lu- label(\"%s\")\n", token.line + 1, token.str_val);
        } else if(token.type == TOK_IDENTIFIER) {
            printf("Line %-4lu- identifier(\"%s\")\n", token.line + 1, token.str_val);
        } else if(token.type == TOK_PUSH) {
            printf("Line %-4lu- push(%u)\n", token.line + 1, token.int_val);
        } else {
            printf("Line %-4lu- %s\n", token.line + 1, token_string_from_type(token.type));
        }
    }

    free(source);
    lexer_free_tokens(&lexer);
    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>

#include "codegen.h"
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
    free(source);   // No longer needed

    if(!tokens) {
        return EXIT_FAILURE;
    }

    if(!codegen_generate_binary("out.smol", tokens)) {
        lexer_free_tokens(&lexer);
        return EXIT_FAILURE;
    }

    lexer_free_tokens(&lexer);
    return EXIT_SUCCESS;
}

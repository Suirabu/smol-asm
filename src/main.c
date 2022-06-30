#include <stdio.h>
#include <stdlib.h>

#include "args.h"
#include "codegen.h"
#include "error.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "util.h"

#include "strmap.h"

int main(int argc, char** argv) {
    Options options = args_parse(argc, argv);
    if(!options.input_path) {
        REPORT_ERROR("No input file provided\n");
        return EXIT_FAILURE;
    }

    // Set output path if not already set
    if(!options.output_path) {
        options.output_path = "out.smol";
    }

    char* source = util_get_file_contents(options.input_path);
    if(!source) {
        return EXIT_FAILURE;
    }

    Lexer lexer = { .source = source };
    Token* tokens = lexer_collect_tokens(&lexer);
    free(source);   // No longer needed

    if(!tokens) {
        return EXIT_FAILURE;
    }

    ParseResult parse_result = parse_tokens(tokens);
    if(!parse_result.instructions) {
        lexer_free_tokens(&lexer);
        return EXIT_FAILURE;
    }
    lexer_free_tokens(&lexer);

    codegen_init(options.output_path);
    codegen_generate_binary(parse_result.instructions, parse_result.len);
    free_instructions();
    codegen_deinit();

    return EXIT_SUCCESS;
}

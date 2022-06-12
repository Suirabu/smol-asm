#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "codegen.h"
#include "error.h"
#include "token.h"

static FILE* output_file = NULL;
static const char* output_path = NULL;

bool codegen_init(const char* path) {
    output_file = fopen(path, "wb");
    output_path = path;
    return output_file != NULL;
}

void codegen_deinit(void) {
    fclose(output_file);
    output_file = NULL;
    output_path = NULL;
}

static void write_char_to_output_file(char c) {
    if(fputc(c, output_file) == EOF) {
        REPORT_ERROR("Failed to write to output file\n");
        exit(EXIT_FAILURE);
    }
}

bool codegen_generate_binary(Token *tokens) {
    assert(output_file != NULL);
    bool has_error = false;

    while(tokens->type != TOK_EOF) {
        const Token token = *tokens++;
        const TokenType type = token.type;

        switch(type) {
            case TOK_PUSH: {
                const Token number = *tokens++;

                if(number.type != TOK_NUMBER) {
                    REPORT_ERROR_AT_LINE("Expected number literal after keyword 'push', found '%s' instead\n",
                        number.line + 1,
                        token_string_from_type(number.type)
                    );
                    has_error = true;
                    break;
                }

                const TokenType op_type = number.int_val > 0xFF ? TOK_PUSH_16 : TOK_PUSH_8;
                write_char_to_output_file(op_type);

                if(op_type == TOK_PUSH_8) {
                    uint8_t int_val = number.int_val;
                    write_char_to_output_file(int_val);
                } else {
                    uint8_t msb = (number.int_val & 0xFF00) >> 8;
                    uint8_t lsb = number.int_val & 0x00FF;
                    write_char_to_output_file(msb);
                    write_char_to_output_file(lsb);
                }
                break;
            }
            
            case TOK_JMP:
            case TOK_JEQ:
            case TOK_JNE:
            case TOK_JLT:
            case TOK_JLE:
            case TOK_JGT:
            case TOK_JGE:
            {
                const Token number = *tokens++;

                if(number.type != TOK_NUMBER) {
                    REPORT_ERROR_AT_LINE("Expected number literal after keyword '%s', found '%s' instead\n",
                        number.line + 1,
                        token_string_from_type(type),
                        token_string_from_type(number.type)
                    );
                    has_error = true;
                    break;
                }

                write_char_to_output_file(type);

                uint8_t msb = (number.int_val & 0xFF00) >> 8;
                uint8_t lsb = number.int_val & 0x00FF;
                write_char_to_output_file(msb);
                write_char_to_output_file(lsb);
                break;

            }

            case TOK_LABEL:
            case TOK_IDENTIFIER:
                REPORT_ERROR("Labels have not yet been implemented\n");
                has_error = true;
                break;

            case TOK_NUMBER:
                REPORT_ERROR_AT_LINE("Encountered number literal '%u' outside of its proper context\n", token.line + 1, token.int_val);
                has_error = true;
                break;

            default:
                write_char_to_output_file(type);
                break;
        }
    }

    if(has_error) { remove(output_path); }
    return !has_error;
}

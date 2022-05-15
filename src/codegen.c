#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#include "codegen.h"
#include "error.h"
#include "token.h"

bool codegen_generate_binary(const char *output_path, Token *tokens) {
    bool result = true;

    FILE* bin = fopen(output_path, "wb");
    if(!bin) {
        REPORT_ERROR("Failed to open file '%s' for writing\n", output_path);
        goto cleanup_err;
    }

    while(tokens->type != TOK_EOF) {
        const Token token = *tokens++;
        const TokenType type = token.type;

        // Direct keyword
        if(type < DIRECT_KEYWORD_END) {
            assert(fputc(type, bin) != EOF);
        } else if(type == TOK_PUSH) {
            const Token number = *tokens++;

            if(number.type != TOK_NUMBER) {
                REPORT_ERROR_AT_LINE("Expected number literal after keyword 'push', found '%s' instead\n", number.line + 1, token_string_from_type(number.type));
                goto cleanup_err;
            }

            const TokenType op_type = number.int_val > 0xFF ? TOK_PUSH_16 : TOK_PUSH_8;
            assert(fputc(op_type, bin) != EOF);

            if(op_type == TOK_PUSH_8) {
                uint8_t int_val = number.int_val;
                assert(fputc(int_val, bin) != EOF);
            } else {
                uint8_t msb = (number.int_val & 0xFF00) >> 8;
                uint8_t lsb = number.int_val & 0x00FF;
                assert(fputc(msb, bin) != EOF);
                assert(fputc(lsb, bin) != EOF);
            }
        } else if(type == TOK_LABEL || type == TOK_IDENTIFIER) {
            REPORT_ERROR("Labels have not yet been implemented\n");
            goto cleanup_err;
        } else if(type == TOK_NUMBER) {
            REPORT_ERROR_AT_LINE("Encountered number literal '%u' outside of its proper context\n", token.line + 1, token.int_val);
            goto cleanup_err;
        }
    }

    goto cleanup;
cleanup_err:
    result = false;
    if (bin) { remove(output_path); }
cleanup:
    if (bin) { fclose(bin); }
    return result;
}

#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#include "codegen.h"
#include "error.h"
#include "token.h"

uint8_t get_opcode_from_token_type(TokenType type) {
    switch(type) {
        case TOK_NOP:       return 0x00;
        case TOK_DROP:      return 0x12;
        case TOK_DUP:       return 0x13;
        case TOK_SWAP:      return 0x14;
        case TOK_OVER:      return 0x15;
        case TOK_ROT:       return 0x16;
        case TOK_STORE_8:   return 0x17;
        case TOK_STORE_16:  return 0x18;
        case TOK_LOAD_8:    return 0x19;
        case TOK_LOAD_16:   return 0x1A;
        case TOK_ADD:       return 0x20;
        case TOK_SUB:       return 0x21;
        case TOK_MULT:      return 0x22;
        case TOK_DIV:       return 0x23;
        case TOK_MOD:       return 0x24;
        case TOK_AND:       return 0x30;
        case TOK_OR:        return 0x31;
        case TOK_NOT:       return 0x32;
        case TOK_XOR:       return 0x33;
        case TOK_SHIFTL:    return 0x34;
        case TOK_SHIFTR:    return 0x35;
        case TOK_CMP:       return 0x40;
        case TOK_JMP:       return 0x41;
        case TOK_JEQ:       return 0x42;
        case TOK_JNE:       return 0x43;
        case TOK_JLT:       return 0x44;
        case TOK_JLE:       return 0x45;
        case TOK_JGT:       return 0x46;
        case TOK_JGE:       return 0x47;
    }

    assert(false && "Encountered token type with unknown opcode");
}

bool codegen_generate_binary(const char *output_path, Token *tokens) {
    FILE* bin = fopen(output_path, "wb");
    if(!bin) {
        REPORT_ERROR("Failed to open file '%s' for writing\n", output_path);
        return false;
    }

    while(tokens->type != TOK_EOF) {
        Token token = *tokens++;
        if(token.type == TOK_PUSH) {
            // push16
            if(token.int_val > 0xFF) {
                assert(fputc(0x11, bin) != EOF);
                assert(fwrite(&token.int_val, 2, 1, bin) == 1);
            }
            // push8
            else {
                assert(fputc(0x10, bin) != EOF);
                // Avoid endianness issues
                char val = token.int_val;
                assert(fputc(val, bin) != EOF);
            }
        } else if(token.type == TOK_LABEL || token.type == TOK_IDENTIFIER) {
            REPORT_ERROR("Label codegen has not been implemented yet!\n");
            return false;
        } else {
            assert(fputc(get_opcode_from_token_type(token.type), bin) != EOF);
        }
    }

    fclose(bin);
    return true;
}

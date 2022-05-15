#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    // Direct keywords
    // Keywords which have a direct coorespondance to an opcode
    TOK_NOP         = 0x00,

    TOK_PUSH_8      = 0x10,
    TOK_PUSH_16     = 0x11,
    TOK_DROP        = 0x12,
    TOK_DUP         = 0x13,
    TOK_SWAP        = 0x14,
    TOK_OVER        = 0x15,
    TOK_ROT         = 0x16,
    TOK_STORE_8     = 0x17,
    TOK_STORE_16    = 0x18,
    TOK_LOAD_8      = 0x19,
    TOK_LOAD_16     = 0x1A,

    TOK_ADD         = 0x20,
    TOK_SUB         = 0x21,
    TOK_MULT        = 0x22,
    TOK_DIV         = 0x23,
    TOK_MOD         = 0x24,

    TOK_AND         = 0x30,
    TOK_OR          = 0x31,
    TOK_NOT         = 0x32,
    TOK_XOR         = 0x33,
    TOK_SHIFTL      = 0x34,
    TOK_SHIFTR      = 0x35,

    TOK_CMP         = 0x40,
    TOK_JMP         = 0x41,
    TOK_JEQ         = 0x42,
    TOK_JNE         = 0x43,
    TOK_JLT         = 0x44,
    TOK_JLE         = 0x45,
    TOK_JGT         = 0x46,
    TOK_JGE         = 0x47,

    // Somewhat hacky way to mark the end of the direct keywords
    // This is useful for code generation since any type with a value lower than this has the value
    // of its cooresponding opcode, and can thus be used directly when mapping types to opcodes
    DIRECT_KEYWORD_END,     

    // Non-direct keywords
    // Keywords which *do not* have a direct coorespondance to an opcode
    TOK_PUSH,

    // Meta types
    // Types which do not have any behavior outside of compilation
    TOK_LABEL,
    TOK_IDENTIFIER,
    TOK_NUMBER,

    TOK_UNKNOWN,
    TOK_EOF,
} TokenType;

typedef struct {
    TokenType type;

    union {
        uint16_t int_val;
        char* str_val;
    };
    
    size_t line;
} Token;

TokenType token_type_from_string(const char* str);
const char* token_string_from_type(TokenType type);

#endif // TOKEN_H

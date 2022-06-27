#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    TOK_NOP,

    TOK_PUSH,
    TOK_DROP,
    TOK_DUP,
    TOK_SWAP,
    TOK_OVER,
    TOK_ROT,
    TOK_STORE_8,
    TOK_STORE_16,
    TOK_LOAD_8,
    TOK_LOAD_16,

    TOK_ADD,
    TOK_SUB,
    TOK_MULT,
    TOK_DIV,
    TOK_MOD,

    TOK_AND,
    TOK_OR,
    TOK_NOT,
    TOK_XOR,
    TOK_SHIFTL,
    TOK_SHIFTR,

    TOK_CMP,
    TOK_JMP,
    TOK_JEQ,
    TOK_JNE,
    TOK_JLT,
    TOK_JLE,
    TOK_JGT,
    TOK_JGE,

    TOK_LABEL,
    TOK_IDENTIFIER,
    TOK_NUMBER,

    TOK_UNKNOWN,
    TOK_EOF,
} TokenType;

typedef struct {
    TokenType type;
    char* lexemme;
    uint16_t val;
    size_t line;
} Token;

TokenType token_type_from_string(const char* str);
const char* token_string_from_type(TokenType type);

#endif // TOKEN_H

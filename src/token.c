#include <stdbool.h>
#include <string.h>

#include "error.h"
#include "token.h"

typedef struct {
    TokenType type;
    const char* str;
} TypeStringPair;

static TypeStringPair type_string_pairs[] = {
    { TOK_NOP, "nop" },
    { TOK_PUSH, "push" },
    { TOK_DROP, "drop" },
    { TOK_DUP, "dup" },
    { TOK_SWAP, "swap" },
    { TOK_OVER, "over" },
    { TOK_ROT, "rot" },
    { TOK_STORE_8, "store8" },
    { TOK_STORE_16, "store16" },
    { TOK_LOAD_8, "load8" },
    { TOK_LOAD_16, "load16" },
    { TOK_ADD, "add" },
    { TOK_SUB, "sub" },
    { TOK_MULT, "mult" },
    { TOK_DIV, "div" },
    { TOK_MOD, "mod" },
    { TOK_AND, "and" },
    { TOK_OR, "or" },
    { TOK_NOT, "not" },
    { TOK_XOR, "xor" },
    { TOK_SHIFTL, "shiftl" },
    { TOK_SHIFTR, "shiftr" },
    { TOK_CMP, "cmp" },
    { TOK_JMP, "jmp" },
    { TOK_JEQ, "jeq" },
    { TOK_JNE, "jne" },
    { TOK_JLT, "jlt" },
    { TOK_JLE, "jle" },
    { TOK_JGT, "jgt" },
    { TOK_JGE, "jgt" },
};
const size_t type_string_pairs_len = sizeof(type_string_pairs) / sizeof(TypeStringPair);

static bool is_label(const char* lexemme) {
    const size_t lexemme_len = strlen(lexemme);
    return lexemme[lexemme_len - 1] == ':';
}

TokenType token_type_from_string(const char* str) {
    for(size_t i = 0; i < type_string_pairs_len; ++i) {
        const char* pair_str = type_string_pairs[i].str;
        if(strcmp(pair_str, str) == 0) {
            return type_string_pairs[i].type;
        }
    }

    return is_label(str) ? TOK_LABEL : TOK_IDENTIFIER;
}

const char* token_string_from_type(TokenType type) {
    for(size_t i = 0; i < type_string_pairs_len; ++i) {
        TokenType pair_type = type_string_pairs[i].type;
        if(type == pair_type) {
            return type_string_pairs[i].str;
        }
    }

    return NULL;
}

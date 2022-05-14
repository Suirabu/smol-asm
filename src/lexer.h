#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

#include "token.h"

typedef struct {
    char* source;
    size_t sp;

    size_t line;

    Token* tokens;
    size_t tokens_len;
} Lexer;

Token* lexer_collect_tokens(Lexer* lexer);
void lexer_free_tokens(Lexer* lexer);

#endif // LEXER_H

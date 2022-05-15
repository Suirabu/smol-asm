#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "lexer.h"
#include "token.h"

#define REPORT_ERROR_AT_LINE(...) \
    REPORT_ERROR("\033[33;1mline %lu:\033[0m " __VA_ARGS__)

static char peek(Lexer* lexer) {
    return lexer->source[lexer->sp];
}

static char advance(Lexer* lexer) {
    if(peek(lexer) == '\n') {
        ++lexer->line;
    }

    return lexer->source[lexer->sp++];
}

static bool reached_end(Lexer* lexer) {
    return peek(lexer) == '\0';
}

// Returns false if null character is encountered, otherwise true
static bool skip_whitespace(Lexer* lexer) {
    while(!reached_end(lexer) && isspace(peek(lexer))) {
        advance(lexer);
    }

    return !reached_end(lexer);
}

static bool skip_lexemme(Lexer* lexer) {
    while(!reached_end(lexer) && !isspace(peek(lexer))) {
        advance(lexer);
    }

    return !reached_end(lexer);
}

static void append_token(Lexer* lexer, Token token) {
    lexer->tokens = realloc(lexer->tokens, ++lexer->tokens_len * sizeof(Token));
    assert(lexer->tokens != NULL && "After reallocating tokens array");
    lexer->tokens[lexer->tokens_len - 1] = token;
}

static bool get_lexemme(Lexer* lexer, char lexemme[MAX_LEXEMME_LEN]) {
    size_t lp = 0;

    while(!reached_end(lexer) && !isspace(peek(lexer)) && lp < MAX_LEXEMME_LEN - 1) {
        lexemme[lp++] = advance(lexer);
    }

    // Lexemme length surpassed our maximum lexemme length
    if(lp == MAX_LEXEMME_LEN - 1) {
        return false;
    }

    lexemme[lp] = '\0';
    return true;
}

// Returns false on failure to collect a token, otherwise true
static bool collect_token(Lexer* lexer) {
    char lexemme[MAX_LEXEMME_LEN];
    if(!get_lexemme(lexer, lexemme)) {
        REPORT_ERROR_AT_LINE("Lexemme surpassed maximum length of %u\n", lexer->line + 1, MAX_LEXEMME_LEN - 1);
        skip_lexemme(lexer);
        return false;
    }

    if(!isalpha(lexemme[0])) {
        REPORT_ERROR_AT_LINE("Invalid lexemme '%s'\n", lexer->line + 1, lexemme);
        return false;
    }

    TokenType type = token_type_from_string(lexemme);
    
    if(type == TOK_LABEL || type == TOK_IDENTIFIER) {
        // Subtract one from lexemme length if our token is a label, effectively chopping off the
        // trailing colon
        const size_t lexemme_len = strlen(lexemme) - (type == TOK_LABEL ? 1 : 0);

        // Collect identifier
        char* lexemme_buffer = malloc(lexemme_len + 1);
        assert(lexemme_buffer != NULL);

        assert(memcpy(lexemme_buffer, lexemme, lexemme_len) != NULL);
        lexemme_buffer[lexemme_len] = '\0';

        Token token = {
            .type = type,
            .str_val = lexemme_buffer,
            .line = lexer->line,
        };
        append_token(lexer, token);
        return true;
    }

    // Push operation (must be preceded by a number literal)
    if(type == TOK_PUSH) {
        if(!skip_whitespace(lexer)) {
            REPORT_ERROR_AT_LINE("Expected number after push operation, found end of file instead\n", lexer->line + 1);
        }

        if(!get_lexemme(lexer, lexemme)) {
            REPORT_ERROR_AT_LINE("Lexemme surpassed maximum length of %u\n", lexer->line + 1, MAX_LEXEMME_LEN - 1);
            skip_lexemme(lexer);
            return false;
        }
        
        const size_t lexemme_len = strlen(lexemme);
        for(size_t i = 0; i < lexemme_len; ++i) {
            if(!isdigit(lexemme[i])) {
                REPORT_ERROR_AT_LINE("Expected number after push operation, found '%s' instead\n", lexer->line + 1, lexemme);
                return false;
            }
        }

        const int int_val = atoi(lexemme);
        if(int_val > USHRT_MAX) {
            REPORT_ERROR_AT_LINE("Number literal %u is too large\n", lexer->line + 1, int_val);
            return false;
        }

        Token token = {
            .type = type,
            .int_val = int_val,
            .line = lexer->line,
        };
        append_token(lexer, token);
        return true;
    }

    Token token = {
        .type = type,
        .line = lexer->line,
    };
    append_token(lexer, token);
    return true;
}

Token* lexer_collect_tokens(Lexer* lexer) {
    bool contains_error = false;

    while(!reached_end(lexer)) {
        // Start at beginning of word
        if(!skip_whitespace(lexer)) {
            break;  // Reached end of file
        }

        if(!collect_token(lexer)) {
            contains_error = true;
        }
    }

    if(contains_error) {
        lexer_free_tokens(lexer);
        return NULL;
    }

    Token token = {
        .type = TOK_EOF,
        .line = lexer->line,
    };
    append_token(lexer, token);

    return lexer->tokens;
}

void lexer_free_tokens(Lexer* lexer) {
    for(size_t i = 0; i < lexer->tokens_len; ++i) {
        const Token token = lexer->tokens[i];
        if(token.type == TOK_LABEL) {
            free(token.str_val);
        }
    }
    free(lexer->tokens);
}

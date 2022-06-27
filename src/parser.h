#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>

#include "instruction.h"
#include "token.h"

typedef struct {
    Instruction* instructions;
    size_t len;
} ParseResult;

ParseResult parse_tokens(Token* tokens);
void free_instructions(void);

#endif // PARSER_H

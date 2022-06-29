#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "error.h"
#include "parser.h"
#include "instruction.h"
#include "strmap.h"
#include "token.h"

static Instruction* instructions = NULL;
static StrMap map = { 0 };

static void append_instruction(Instruction inst, size_t len) {
    instructions = realloc(instructions, len * sizeof(Instruction));
    assert(instructions != NULL);
    instructions[len - 1] = inst;
}

static void collect_constants(Token* tokens) {
    uint16_t pc = 0x2000;

    while(tokens->type != TOK_EOF) {
        const Token token = *tokens++;

        while(tokens->type != TOK_EOF) {
            Token token = *tokens++;

            switch(token.type) {
                // pc += 0
                case TOK_LABEL:
                    strmap_add(&map, token.lexemme, pc);
                    break;
                case TOK_NUMBER:
                case TOK_IDENTIFIER:
                    break;

                // pc += 2 : 3
                case TOK_PUSH:
                case TOK_JMP:
                case TOK_JEQ:
                case TOK_JNE:
                case TOK_JLT:
                case TOK_JLE:
                case TOK_JGT:
                case TOK_JGE: {
                    const Opcode opcode = opcode_from_token(token);
                    pc += opcode == OP_PUSH_8 ? 2 : 3;
                    break;
                }

                // pc += 1
                default:
                    ++pc;
                    break;
            }
        }
    }
}

ParseResult parse_tokens(Token* tokens) {
    size_t instructions_len = 0;
    bool has_error = false;

    collect_constants(tokens);

    while(tokens->type != TOK_EOF) {
        Token token = *tokens++;

        switch(token.type) {
            case TOK_LABEL:
                break;
            case TOK_IDENTIFIER:
                REPORT_ERROR_AT_LINE("Unexpected identifier '%s'\n", token.line + 1, token.lexemme);
                has_error = true;
                break;
            case TOK_NUMBER:
                REPORT_ERROR_AT_LINE("Unexpected number literal\n", token.line + 1);
                has_error = true;
                break;

            case TOK_PUSH:
            case TOK_JMP:
            case TOK_JEQ:
            case TOK_JNE:
            case TOK_JLT:
            case TOK_JLE:
            case TOK_JGT:
            case TOK_JGE: {
                const Opcode opcode = opcode_from_token(token);

                Token operand_token = *tokens++;
                if(operand_token.type != TOK_IDENTIFIER && operand_token.type != TOK_NUMBER) {
                    REPORT_ERROR_AT_LINE(
                        "Expected label or number literal, found '%s' instead\n",
                        operand_token.line + 1,
                        token_string_from_type(operand_token.type)
                    );
                    has_error = true;
                    break;
                }

                uint16_t operand;

                if(operand_token.type == TOK_NUMBER) {
                    if(token.type != TOK_PUSH) {
                        REPORT_WARNING_AT_LINE(
                            "Use of number literal where label would be more appropriate\n",
                            operand_token.line + 1
                        );
                    }
                    operand = operand_token.val;
                } else if(!strmap_has_key(&map, operand_token.lexemme)) {
                    REPORT_ERROR_AT_LINE("Label '%s' does not exist\n", operand_token.line + 1, operand_token.lexemme);
                    has_error = true;
                    break;
                } else {
                    operand = strmap_get_value(&map, operand_token.lexemme);
                }

                append_instruction(
                    (Instruction) { .opcode = opcode_from_token(token), .operand = operand },
                    ++instructions_len
                );
                break;
            }

            default:
                append_instruction((Instruction) { .opcode = opcode_from_token(token) }, ++instructions_len);
                break;
        }
    }

    strmap_free(&map);

    if(has_error) {
        free_instructions();
        return (ParseResult) { .instructions = NULL, .len = 0 };
    }
    
    return (ParseResult) { .instructions = instructions, .len = instructions_len };
}

void free_instructions(void) {
    if(instructions) { free(instructions); }
}

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "token.h"

typedef enum {
    OP_NOP         = 0x00,

    OP_PUSH_8      = 0x10,
    OP_PUSH_16     = 0x11,
    OP_DROP        = 0x12,
    OP_DUP         = 0x13,
    OP_SWAP        = 0x14,
    OP_OVER        = 0x15,
    OP_ROT         = 0x16,
    OP_STORE_8     = 0x17,
    OP_STORE_16    = 0x18,
    OP_LOAD_8      = 0x19,
    OP_LOAD_16     = 0x1A,

    OP_ADD         = 0x20,
    OP_SUB         = 0x21,
    OP_MULT        = 0x22,
    OP_DIV         = 0x23,
    OP_MOD         = 0x24,

    OP_AND         = 0x30,
    OP_OR          = 0x31,
    OP_NOT         = 0x32,
    OP_XOR         = 0x33,
    OP_SHIFTL      = 0x34,
    OP_SHIFTR      = 0x35,

    OP_CMP         = 0x40,
    OP_JMP         = 0x41,
    OP_JEQ         = 0x42,
    OP_JNE         = 0x43,
    OP_JLT         = 0x44,
    OP_JLE         = 0x45,
    OP_JGT         = 0x46,
    OP_JGE         = 0x47,
} Opcode;

typedef struct {
    Opcode opcode;
    uint16_t operand;
} Instruction;

Opcode opcode_from_token(Token token);

#endif // INSTRUCTION_H

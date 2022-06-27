#include "instruction.h"
#include "token.h"

Opcode opcode_from_token(Token token) {
    switch(token.type) {
        case TOK_NOP:
            return OP_NOP;

        case TOK_PUSH: {
            if(token.val < 0x100) {
                return OP_PUSH_8;
            } else {
                return OP_PUSH_16;
            }
        }
        case TOK_DROP:
            return OP_DROP;
        case TOK_DUP:
            return OP_DUP;
        case TOK_SWAP:
            return OP_SWAP;
        case TOK_OVER:
            return OP_OVER;
        case TOK_ROT:
            return OP_ROT;
        case TOK_STORE_8:
            return OP_STORE_8;
        case TOK_STORE_16:
            return OP_STORE_16;
        case TOK_LOAD_8:
            return OP_LOAD_8;
        case TOK_LOAD_16:
            return OP_LOAD_16;

        case TOK_ADD:
            return OP_ADD;
        case TOK_SUB:
            return OP_SUB;
        case TOK_MULT:
            return OP_MULT;
        case TOK_DIV:
            return OP_DIV;
        case TOK_MOD:
            return OP_MOD;

        case TOK_AND:
            return OP_AND;
        case TOK_OR:
            return OP_OR;
        case TOK_NOT:
            return OP_NOT;
        case TOK_XOR:
            return OP_XOR;
        case TOK_SHIFTL:
            return OP_SHIFTL;
        case TOK_SHIFTR:
            return OP_SHIFTR;

        case TOK_CMP:
            return OP_CMP;
        case TOK_JMP:
            return OP_JMP;
        case TOK_JEQ:
            return OP_JEQ;
        case TOK_JNE:
            return OP_JNE;
        case TOK_JLT:
            return OP_JLT;
        case TOK_JLE:
            return OP_JLE;
        case TOK_JGT:
            return OP_JGT;
        case TOK_JGE:
            return OP_JGE;
    
        default:
            return -1;
    }
}

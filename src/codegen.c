#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "codegen.h"
#include "error.h"
#include "strmap.h"
#include "token.h"
#include "instruction.h"

static FILE* output_file = NULL;
static const char* output_path = NULL;

bool codegen_init(const char* path) {
    output_file = fopen(path, "wb");
    output_path = path;
    return output_file != NULL;
}

void codegen_deinit(void) {
    fclose(output_file);
    output_file = NULL;
    output_path = NULL;
}

static void write_char_to_output_file(char c) {
    if(fputc(c, output_file) == EOF) {
        REPORT_ERROR("Failed to write to output file\n");
        exit(EXIT_FAILURE);
    }
}

void codegen_generate_binary(Instruction* instructions, size_t len) {
    assert(output_file != NULL);

    for(size_t i = 0; i < len; ++i) {
        Instruction inst = instructions[i];
        write_char_to_output_file(inst.opcode);

        switch(inst.opcode) {
            case OP_PUSH_8:
                write_char_to_output_file(inst.operand);
                break;

            case OP_PUSH_16:
            case OP_JMP:
            case OP_JEQ:
            case OP_JNE:
            case OP_JLT:
            case OP_JLE:
            case OP_JGT:
            case OP_JGE: {
                uint8_t msb = (inst.operand & 0xFF00) >> 8;
                uint8_t lsb = inst.operand & 0x00FF;
                write_char_to_output_file(msb);
                write_char_to_output_file(lsb);
                break;
            }
        }
    }
}

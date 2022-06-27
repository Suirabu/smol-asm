#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdbool.h>

#include "token.h"
#include "instruction.h"

bool codegen_init(const char* output_path);
void codegen_deinit(void);

void codegen_generate_binary(Instruction* instructions, size_t len);

#endif // CODEGEN_H

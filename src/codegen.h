#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdbool.h>

#include "token.h"

bool codegen_init(const char* output_path);
void codegen_deinit(void);

bool codegen_generate_binary(Token* tokens);

#endif // CODEGEN_H

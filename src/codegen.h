#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdbool.h>

#include "token.h"

bool codegen_generate_binary(const char* output_path, Token* tokens);

#endif // CODEGEN_H

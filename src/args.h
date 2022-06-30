#ifndef ARGS_H
#define ARGS_H

#include <stdbool.h>

typedef struct {
    const char* input_path;
    const char* output_path;
} Options;

Options args_parse(int argc, char** argv);

#endif // ARGS_H

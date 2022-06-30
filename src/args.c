#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "error.h"

void display_help(FILE* stream, const char* exec_path) {
    fprintf(stream,
        "SmolAsm - The Smol assembler\n\n"
        "Usage:\n"
        "    %s [options] <file path>\n\n"
        "Options:\n"
        "    -h, --help     Display help information\n"
        "    -o, --output   Specify output path of generated binary\n",
        exec_path
    );
}

Options args_parse(int argc, char** argv) {
    Options options = { 0 };
    const char* exec_path = argv[0];
    
    if(argc < 2) {
        display_help(stderr, exec_path);
        exit(EXIT_FAILURE);
    }

    // Parse arguments
    for(int i = 1; i < argc; ++i) {
        const char* arg = argv[i];

        if(arg[0] == '-') {
            // Long flag
            if(arg[1] == '-') {
                if(strcmp(arg, "--help") == 0) {
                    display_help(stdout, exec_path);
                    exit(EXIT_SUCCESS);
                }
                else if(strcmp(arg, "--output") == 0) {
                    if(i == argc - 1) {
                        REPORT_ERROR("Expected output file path, found nothing instead.\n");
                        exit(EXIT_FAILURE);
                    }
                    options.output_path = argv[++i];
                }
                else {
                    REPORT_ERROR("Unknown option '%s' provided.\n", arg);
                    exit(EXIT_FAILURE);
                }
            }
            // Short flag
            else {
                ++arg;  // Skip leading dash
                while(*arg != '\0') {
                    const char c = *arg++;

                    switch(c) {
                    case 'h':
                        display_help(stdout, exec_path);
                        exit(EXIT_SUCCESS);
                        break;
                    case 'o':
                        if(i == argc - 1) {
                            REPORT_ERROR("Expected output file path, found nothing instead.\n");
                            exit(EXIT_FAILURE);
                        }
                        options.output_path = argv[++i];
                        break;

                    default:
                        REPORT_ERROR("Unknown option '-%c' provided.\n", c);
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
        // Additional arguments
        else {
            if(!options.input_path) {
                options.input_path = arg;
            }
            else {
                REPORT_ERROR("Unexpected argument '%s' provided.\n", arg);
            }
        }
    }

    return options;
}

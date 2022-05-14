#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "util.h"

int main(int argc, char** argv) {
    if(argc < 2) {
        REPORT_ERROR("No input file provided\n");
        return EXIT_FAILURE;
    }

    const char* source_path = argv[1];
    
    char* source = util_get_file_contents(source_path);
    if(!source) {
        return EXIT_FAILURE;
    }

    printf("File contents:\n%s\n", source);
    free(source);

    return EXIT_SUCCESS;
}

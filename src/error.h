#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>

#define REPORT_ERROR(...) \
    fprintf(stderr, "\033[31;1mError:\033[0m " __VA_ARGS__);
#define REPORT_WARNING(...) \
    fprintf(stderr, "\033[33;1mWarning:\033[0m " __VA_ARGS__);

#define REPORT_ERROR_AT_LINE(...) \
    REPORT_ERROR("line %lu: " __VA_ARGS__)

#define REPORT_WARNING_AT_LINE(...) \
    REPORT_WARNING("line %lu: " __VA_ARGS__)

#endif // ERROR_H

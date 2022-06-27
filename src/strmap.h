#ifndef STRMAP_H
#define STRMAP_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    char** keys;
    uint16_t* vals;
    size_t count;
} StrMap;

void strmap_add(StrMap* map, char* str, uint16_t val);
void strmap_free(StrMap* map);
bool strmap_has_key(StrMap* map, char* key);
uint16_t strmap_get_value(StrMap* map, char* key);

#endif

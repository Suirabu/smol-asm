#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "strmap.h"

void strmap_add(StrMap* map, char* key, uint16_t val) {
    ++map->count;
    map->keys = realloc(map->keys, map->count * sizeof(char*));
    assert(map->keys != NULL);

    map->vals = realloc(map->vals, map->count * sizeof(uint16_t));
    assert(map->vals != NULL);

    map->keys[map->count - 1] = key;
    map->vals[map->count - 1] = val;
}

bool strmap_has_key(StrMap* map, char* key) {
    for(size_t i = 0; i < map->count; ++i) {
        if(strcmp(map->keys[i], key) == 0) {
            return true;
        }
    }

    return false;
}

uint16_t strmap_get_value(StrMap* map, char* key) {
    for(size_t i = 0; i < map->count; ++i) {
        if(strcmp(map->keys[i], key) == 0) {
            return map->vals[i];
        }
    }

    return 0;
}

void strmap_free(StrMap* map) {
    free(map->keys);
    free(map->vals);
}

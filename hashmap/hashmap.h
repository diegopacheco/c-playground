#ifndef HASHMAP_H
#define HASHMAP_H

#include <string.h>
#include <limits.h>

#define HASHMAP_SIZE 100

typedef struct hashmap_entry {
    char *key;
    char *value;
    struct hashmap_entry *next;
} hashmap_entry;

typedef struct {
    hashmap_entry **entries;
} hashmap;

unsigned int hash_function(const char *key);
hashmap *hashmap_init();
void hashmap_put(hashmap *map, const char *key, const char *value);
char *hashmap_get(hashmap *map, const char *key);
void hashmap_free(hashmap *map);

#endif
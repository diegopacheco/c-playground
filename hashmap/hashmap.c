#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include "hashmap.h"

unsigned int hash_function(const char *key) {
    unsigned long int hash = 0;
    int i = 0;
    while (hash < ULONG_MAX && i < strlen(key)) {
        hash = hash << 8;
        hash += key[i];
        i++;
    }
    return hash % HASHMAP_SIZE;
}

hashmap *hashmap_init() {
    hashmap *map = malloc(sizeof(hashmap));
    map->entries = malloc(sizeof(hashmap_entry*) * HASHMAP_SIZE);
    for (int i = 0; i < HASHMAP_SIZE; i++) {
        map->entries[i] = NULL;
    }
    return map;
}

void hashmap_put(hashmap *map, const char *key, const char *value) {
    unsigned int slot = hash_function(key);
    hashmap_entry *entry = map->entries[slot];

    if (entry == NULL) {
        entry = malloc(sizeof(hashmap_entry));
        entry->key = strdup(key);
        entry->value = strdup(value);
        entry->next = NULL;
        map->entries[slot] = entry;
    } else {
        hashmap_entry *prev;
        while (entry != NULL) {
            if (strcmp(entry->key, key) == 0) {
                free(entry->value);
                entry->value = strdup(value);
                return;
            }
            prev = entry;
            entry = entry->next;
        }
        entry = malloc(sizeof(hashmap_entry));
        entry->key = strdup(key);
        entry->value = strdup(value);
        entry->next = NULL;
        prev->next = entry;
    }
}

char *hashmap_get(hashmap *map, const char *key) {
    unsigned int slot = hash_function(key);
    hashmap_entry *entry = map->entries[slot];

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

void hashmap_free(hashmap *map) {
    for (int i = 0; i < HASHMAP_SIZE; i++) {
        hashmap_entry *entry = map->entries[i];
        while (entry != NULL) {
            hashmap_entry *temp = entry;
            entry = entry->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }
    free(map->entries);
    free(map);
}
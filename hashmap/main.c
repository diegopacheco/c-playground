#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

int main() {
    hashmap *map = hashmap_init();
    hashmap_put(map, "key1", "value1");
    hashmap_put(map, "key2", "value2");

    printf("key1: %s\n", hashmap_get(map, "key1"));
    printf("key2: %s\n", hashmap_get(map, "key2"));

    hashmap_free(map);
    return 0;
}
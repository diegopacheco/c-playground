#include <stdio.h>
#include "bitset.h"

int main() {
    size_t size = 32;
    BitSet *bitset = bitset_create(size);

    if (!bitset) {
        fprintf(stderr, "Failed to create bitset\n");
        return 1;
    }

    bitset_set(bitset, 5);
    bitset_set(bitset, 10);

    printf("Bit 5: %d\n", bitset_test(bitset, 5));
    printf("Bit 10: %d\n", bitset_test(bitset, 10));
    printf("Bit 15: %d\n", bitset_test(bitset, 15));

    bitset_clear(bitset, 5);
    printf("Bit 5 after clearing: %d\n", bitset_test(bitset, 5));

    bitset_free(bitset);
    return 0;
}
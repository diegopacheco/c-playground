#include "bitset.h"
#include <stdlib.h>
#include <string.h>

BitSet *bitset_create(size_t size) {
    BitSet *bitset = (BitSet *)malloc(sizeof(BitSet));
    if (!bitset) {
        return NULL;
    }
    bitset->size = size;
    bitset->bits = (unsigned char *)calloc((size + 7) / 8, sizeof(unsigned char));
    if (!bitset->bits) {
        free(bitset);
        return NULL;
    }
    return bitset;
}

void bitset_free(BitSet *bitset) {
    if (bitset) {
        free(bitset->bits);
        free(bitset);
    }
}

void bitset_set(BitSet *bitset, size_t index) {
    if (index < bitset->size) {
        bitset->bits[index / 8] |= (1 << (index % 8));
    }
}

void bitset_clear(BitSet *bitset, size_t index) {
    if (index < bitset->size) {
        bitset->bits[index / 8] &= ~(1 << (index % 8));
    }
}

int bitset_test(const BitSet *bitset, size_t index) {
    if (index < bitset->size) {
        return bitset->bits[index / 8] & (1 << (index % 8));
    }
    return 0;
}
#ifndef BITSET_H
#define BITSET_H

#include <stddef.h>

typedef struct {
    unsigned char *bits;
    size_t size;
} BitSet;

BitSet *bitset_create(size_t size);
void bitset_free(BitSet *bitset);
void bitset_set(BitSet *bitset, size_t index);
void bitset_clear(BitSet *bitset, size_t index);
// Check if a bit is set
int bitset_test(const BitSet *bitset, size_t index);

#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define BLOOM_SIZE 1000
#define NUM_HASHES 3

typedef struct {
    uint8_t *bits;
    size_t size;
    int num_hashes;
} BloomFilter;

uint32_t hash1(const char *str) {
    uint32_t hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

uint32_t hash2(const char *str) {
    uint32_t hash = 0;
    int c;
    while ((c = *str++)) {
        hash = hash * 31 + c;
    }
    return hash;
}

uint32_t hash3(const char *str) {
    uint32_t hash = 0;
    int c;
    while ((c = *str++)) {
        hash = (hash << 4) + (hash << 10) + c;
    }
    return hash;
}

uint32_t (*hash_functions[NUM_HASHES])(const char*) = {hash1, hash2, hash3};

BloomFilter* create_bloom_filter(size_t size, int num_hashes) {
    BloomFilter *bf = malloc(sizeof(BloomFilter));
    bf->size = size;
    bf->num_hashes = num_hashes;
    bf->bits = calloc((size + 7) / 8, sizeof(uint8_t));
    return bf;
}

void set_bit(BloomFilter *bf, size_t index) {
    size_t byte_index = index / 8;
    size_t bit_index = index % 8;
    bf->bits[byte_index] |= (1 << bit_index);
}

int get_bit(BloomFilter *bf, size_t index) {
    size_t byte_index = index / 8;
    size_t bit_index = index % 8;
    return (bf->bits[byte_index] & (1 << bit_index)) != 0;
}

void bloom_add(BloomFilter *bf, const char *item) {
    for (int i = 0; i < bf->num_hashes; i++) {
        uint32_t hash = hash_functions[i](item);
        size_t index = hash % bf->size;
        set_bit(bf, index);
    }
}

int bloom_check(BloomFilter *bf, const char *item) {
    for (int i = 0; i < bf->num_hashes; i++) {
        uint32_t hash = hash_functions[i](item);
        size_t index = hash % bf->size;
        if (!get_bit(bf, index)) {
            return 0;
        }
    }
    return 1;
}

void free_bloom_filter(BloomFilter *bf) {
    free(bf->bits);
    free(bf);
}

int main() {
    BloomFilter *bf = create_bloom_filter(BLOOM_SIZE, NUM_HASHES);

    printf("Adding items to bloom filter...\n");
    bloom_add(bf, "apple");
    bloom_add(bf, "banana");
    bloom_add(bf, "cherry");
    bloom_add(bf, "date");
    bloom_add(bf, "elderberry");

    printf("Items added: apple, banana, cherry, date, elderberry\n\n");

    printf("Testing membership:\n");
    printf("apple: %s\n", bloom_check(bf, "apple") ? "possibly in set" : "definitely not in set");
    printf("banana: %s\n", bloom_check(bf, "banana") ? "possibly in set" : "definitely not in set");
    printf("cherry: %s\n", bloom_check(bf, "cherry") ? "possibly in set" : "definitely not in set");
    printf("grape: %s\n", bloom_check(bf, "grape") ? "possibly in set" : "definitely not in set");
    printf("kiwi: %s\n", bloom_check(bf, "kiwi") ? "possibly in set" : "definitely not in set");
    printf("elderberry: %s\n", bloom_check(bf, "elderberry") ? "possibly in set" : "definitely not in set");
    printf("fig: %s\n", bloom_check(bf, "fig") ? "possibly in set" : "definitely not in set");

    printf("\nTesting false positive rate with random strings...\n");
    int false_positives = 0;
    int tests = 100;
    char test_str[20];

    for (int i = 0; i < tests; i++) {
        sprintf(test_str, "random_%d", i + 1000);
        if (bloom_check(bf, test_str)) {
            false_positives++;
        }
    }

    printf("False positives: %d out of %d (%.2f%%)\n",
           false_positives, tests, (float)false_positives / tests * 100);

    free_bloom_filter(bf);

    return 0;
}

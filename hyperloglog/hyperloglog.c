#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include "hyperloglog.h"

uint32_t hash(const char *data) {
    uint32_t hash = HASH_SEED;
    while (*data) {
        hash = (hash * 33) ^ (unsigned char)*data++;
    }
    return hash;
}

HyperLogLog *hll_init(int precision) {
    HyperLogLog *hll = (HyperLogLog *)malloc(sizeof(HyperLogLog));
    hll->precision = precision;
    hll->num_registers = 1 << precision;
    hll->registers = (uint8_t *)calloc(hll->num_registers, sizeof(uint8_t));
    return hll;
}

void hll_add(HyperLogLog *hll, const char *data) {
    uint32_t hash_value = hash(data);
    int index = hash_value >> (32 - hll->precision);
    uint32_t w = (hash_value << hll->precision) | (1 << (hll->precision - 1));
    int leading_zeros = __builtin_clz(w) + 1;
    if (hll->registers[index] < leading_zeros) {
        hll->registers[index] = leading_zeros;
    }
}

double hll_alpha(int m) {
    switch (m) {
        case 16: return 0.673;
        case 32: return 0.697;
        case 64: return 0.709;
        default: return 0.7213 / (1 + 1.079 / m);
    }
}

double hll_estimate(HyperLogLog *hll) {
    double alpha = hll_alpha(hll->num_registers);
    double sum = 0.0;
    for (int i = 0; i < hll->num_registers; i++) {
        sum += 1.0 / (1 << hll->registers[i]);
    }
    double estimate = alpha * hll->num_registers * hll->num_registers / sum;
    return estimate;
}

void hll_free(HyperLogLog *hll) {
    free(hll->registers);
    free(hll);
}
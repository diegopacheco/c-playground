#ifndef hyperloglog_h
#define hyperloglog_h

#include <stdint.h>

#define HASH_SEED 0x12345678

typedef struct {
    int precision;
    int num_registers;
    uint8_t *registers;
} HyperLogLog;

uint32_t hash(const char *data);
HyperLogLog *hll_init(int precision);
void hll_add(HyperLogLog *hll, const char *data);
double hll_alpha(int m);
double hll_estimate(HyperLogLog *hll);
void hll_free(HyperLogLog *hll);

#endif
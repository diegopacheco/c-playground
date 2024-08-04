#include <stdio.h>
#include "hyperloglog.h"

int main() {
    HyperLogLog *hll = hll_init(14);
    hll_add(hll, "hello");
    hll_add(hll, "world");
    hll_add(hll, "hello"); // Duplicate element
    printf("Estimated cardinality: %f\n", hll_estimate(hll));
    hll_free(hll);
    return 0;
}
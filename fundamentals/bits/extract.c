/**
 * extract.c
 * 
 * Extract bits from multi-byte types.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

bool extract_bit(uint32_t v, unsigned int bit) {
    uint32_t mask = 1 << bit;
    return (v & mask) >> bit;
}

int main(int argc, char* argv[]) {
    bool v = extract_bit(1, 1);
    printf("result is %d\n", v);
    return 0;
}

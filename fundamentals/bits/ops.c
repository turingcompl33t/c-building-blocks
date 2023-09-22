/**
 * ops.c
 * 
 * Bitwise operations.
 */

#include <assert.h>
#include <stdbool.h>
#include <limits.h>
#include <stdio.h>

typedef unsigned char byte;

bool set_bit(byte operand, byte bit, byte* result) {
    if (bit > CHAR_BIT - 1) {
        return false;
    }

   *result = operand | (1 << bit);
   return true;
}

bool unset_bit(byte operand, byte bit, byte* result) {
    if (bit > CHAR_BIT - 1) {
        return false;
    } 

    *result = operand & ~(1 << bit);
    return true;
}

bool test_bit(byte operand, byte bit, byte* result) {
    if (bit > CHAR_BIT - 1) {
        return false;
    }

    *result = (operand & (1 << bit)) >> bit;
    return true;
}

int main(int argc, char* argv[]) {
    bool ok;
    byte value = 1;
    byte result = 0;

    ok = set_bit(value, 1, &result);
    if (!ok) {
        return 1;
    }
    assert(result == 3);

    value = result;
    ok = test_bit(value, 1, &result);
    if (!ok) {
        return 1;
    }
    assert(result == 1);

    value = result;
    ok = unset_bit(value, 1, &result);
    if (!ok) {
        return 1;
    }
    assert(result == 1);

    value = result;
    ok = test_bit(value, 1, &result);
    if (!ok) {
        return 1;
    }
    assert(result == 0);
}
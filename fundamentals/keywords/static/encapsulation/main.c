/**
 * main.c
 * A demonstration of `static` for encapsulation.
 */

#include <stdio.h>

#include "lib.h"

int main(int argc, char *argv[]) {
    int r = add(1, 2);
    printf("Result is: %d\n", r);

    // Uncommenting this results in link error
    // r = add_static(1, 2);
    // printf("Result is: %d\n", r);

    return 0;
}


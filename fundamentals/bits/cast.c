/**
 * cast.c
 */

#include <stdio.h>

int main(int argc, char* argv[]) {
    unsigned short a = 0xFF00;
    unsigned char b = (unsigned char)a;

    printf("a = %d\n", a);
    printf("b = %d\n", b);

    return 0;
}

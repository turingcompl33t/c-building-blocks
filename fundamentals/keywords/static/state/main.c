/**
 * main.c
 * Using `static` for maintaining state.
 */

#include <stdio.h>

void callme() {
    int a = 10;
    static int sa = 10;

    a += 5;
    sa += 5;

    printf("a = %d, sa = %d\n", a, sa);
}

int main(int argc, char* argv[]) {
    for (int i = 0; i < 10; ++i) {
        callme();
    }

    return 0;
}

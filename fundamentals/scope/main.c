/**
 * main.c
 * 
 * Playing with C scope.
 */

#include <stdio.h>

void with_declaration() {
    printf("With declaration.\n");

    int a = 1;
    printf("a = %d\n", a);
    {
        printf("a = %d\n", a);
        int a = 2;
        printf("a = %d\n", a);
    }
    printf("a = %d\n", a);
}

void without_declaration() {
    printf("Without declaration.\n");

    int a = 1;
    printf("a = %d\n", a);
    {
        printf("a = %d\n", a);
        a = 2;
        printf("a = %d\n", a);
    }
    printf("a = %d\n", a);
}

int main(int argc, char *argv[]) {
    with_declaration();
    without_declaration();
    return 0;
} 
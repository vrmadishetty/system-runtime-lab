#include <stdio.h>

int main() {
    printf("Hello, Data Structures!\n");
    char p[2][10];
    printf("sizeof two dimensional array: %lu bytes\n", sizeof(p));
    printf("%p\n %p\n", &p[0][0], &p[1][0]);
    return 0;
}
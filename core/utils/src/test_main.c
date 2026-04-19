#include <stdio.h>
#include "bit_utils.h"

int main() {
    
    uint32_t val = 0x04;

    if(bit_is_set(val, 2))
        printf("SET\n");
    else
        printf("NOT SET\n");

    return 0;
}
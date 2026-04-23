#include <stdio.h>

int main()
{
    // multi bit field update - clear consecative bits (bit field) --> clear bits Bit 4 to Bit 7
    // step 1: create width mask
    // step 2: shift into position
    // step 3: Invert and apply
    
    unsigned int REG = 0xffffffff;
    char width = 0x04;

    unsigned int mask = (1 << width)-1;
    printf("mask: 0x%x\n", mask);

    REG &= ~(mask << width);
    printf("REG: 0x%x\n", REG);

    for (width = 1; width <= 8; width++)
    {
        printf("width = %d mask = 0x%X\n", width, (1 << width) - 1);
    }

    return 0;
}

#if 0
    Brain tattoo for consective bit (bit fields) clear
    **  REG &= ~(1 << width) -1) << 4 ) **

    step 1: shift left - One 1 followed by width zeros
        >>>     1 << 4 = 16
                Binary = 0001 0000
    step 2: substract -1
        >>>     (1 << 4) - 1
                Result = 0000 1111
    The core reason "-1" works
        -> Substracting 1 flips all trailing zeros to ones
        Before: 0001 0000
        After substracting: 0000 1111    
    That transformation is entire trick.

    General Pattern
    >>>     ((1 << width) - 1)
    Always produces
    >>>  width number of 1's

    Examples:

    width   Result  Binary
    -----   -----   ------
    1       1       0001
    2       3       0011
    3       7       0111
    4       15      1111
    5       31      11111

#endif
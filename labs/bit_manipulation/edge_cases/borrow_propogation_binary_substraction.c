/*
    concept:
        borrow proprogation in binary substraction

    why it matters:
        Used in ((1 << width) - 1) mask generation
    
    Key learning:
        Substracting 1 from power-of-two creates consecutive 1's
    
    Test:
        width = 4 -> Result = 0x0F
*/

#include <stdio.h>

int main ()
{
    unsigned char mask = (1 << 4) - 1;

    printf("mask: 0x%x\n", mask);

    return 0;
}

#if 0
    step1: mask  1 << 4 gives 0001 0000  i.e 16
    step 2: Substract 1
        we substract 1 from 16

        0001 0000
      - 0000 0001
      -------------

      But the right most bit is 0, so substraction needs borrow.
      So CPU borrow nearest 1, which is:
      Bit 4

      During borrowing:
      0001 0000
      becomes
      0000 1111

      why ?
      The borrowed 1 spreads across zeros.
      Turning them into ones.
      
      0001 0000
    - 0000 0001
    ------------
      0000 1111
    ------------
    Then mask = 0xF

#endif
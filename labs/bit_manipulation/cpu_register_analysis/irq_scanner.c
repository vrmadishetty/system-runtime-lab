/* CPU register analysis (Interrupt Pending register) */
/* Problem statetemnt
    - Given 32 bit interrupt pending register
    - Each bit represent one interrupt line
        - Bit = 1 -> Interrupt pending
        - Bit = 0 -> No interrupt
    
    Write a function that
    1. Finds all the pending interrupt numbers
    2. Prints them in ascending order
    3. Uses efficient bit manipulation
    4. Avoids scanning all 32 bits unnecessarily    */

#include <stdio.h>
#include <stdint.h>


uint32_t process_pending_irqs(uint32_t pending)
{
    uint32_t count = 0;
    while(pending)
    {
        pending &= (pending - 1);
        printf("IRQ pending: 0x%x\n", pending);
        count++;
    }
    printf("interrupts pending count: %d\n", count);
}


int main()
{
    uint32_t IRQ_PENDING = 0x000000B4;
 
    process_pending_irqs(IRQ_PENDING);    

    return 0;
}

#if 0
    BRAIN TATTO
        n & (n - 1) clears the first set bit and can be used to count the set bits
    
    here (n - 1) => Puts the list bit to 0 and trailing 0's to 1's
    n = 8 => 1000
    (n - 1) => 0111
    n & (n - 1) =  0000
#endif

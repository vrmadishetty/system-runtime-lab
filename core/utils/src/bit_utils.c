#include "bit_utils.h"

/*
    U suffix stands for unsigned int. It is used because
    - prevents undefined behaviour in C. In C, shifting a 1 into signed bit (the leftmost bit)
      of a signed integer is "undefined behaviour".
    - Matching types - since function takes uint32_t as the first argument, using 1U keeps the types consistent.
    - Portability - On some 16-bit systems, a regular 1 is only 16 bits wide. Shifting it by 20 positions 
      would fail
*/
uint8_t bit_is_set(uint32_t value, uint8_t pos)
{
    return (value & (1U << pos)) != 0;
}

uint8_t bit_set(uint32_t value, uint8_t pos)
{
    return value | (1U << pos);
}
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

uint8_t bit_clear(uint32_t value, uint8_t pos)
{
    return value & (~(1U << pos));
}

uint32_t multiple_bit_clear(uint32_t value, uint32_t width, uint32_t pos)
{
    uint32_t mask = (1 << width) - 1;
    mask = ~(mask << pos);
    value &= mask;
    return value;
}

uint32_t check_even_odd(uint32_t value)
{
    /* multiply by 2 and divide by 2 --> 1 is 2^1 */
    return (value >> 1) << 1;
}

uint32_t clear_first_set_bit(uint32_t value)
{
    return value & (value - 1);
}

uint32_t position_right_most_set_bit(uint32_t value)
{
    return value & (~(value -1));
}

uint32_t is_power_of_two(uint32_t value)
{
    if (value == 0)
        return 0;

    return ((value & (value - 1)) == 0);
}

uint32_t is_power_of_four(uint32_t n)
{
    /* check the value is not 0*/
    if (n == 0)
        return 0;

    /* must be power of 2, becuase every power of 4 is also power of 2 */
    bool powerof2 = !(n & (n - 1));

    bool mask = !(n & 0xAAAAAAAA);
    return powerof2 && mask;
}

int toggleIthBit(int n, int pos)
{
    int mask = 1 << (pos - 1);
    return (n ^ mask);
}
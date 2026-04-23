#ifndef BIT_UTILS_H
#define BIT_UTILS_H

#include <stdint.h>
#include <stdbool.h>

/* Check if ith bit is set */
uint8_t bit_is_set(uint32_t value, uint8_t pos);

/* set the ith bit */
uint8_t bit_set(uint32_t value, uint8_t pos);

/* clear the ith bit */
uint8_t bit_clear(uint32_t value, uint8_t pos);

/* mutliple consecutive bits clear */
uint32_t multiple_bit_clear(uint32_t value, uint32_t width, uint32_t pos);

/* check whether a number is even or odd */
uint32_t check_even_odd(uint32_t value);

/* clear the first set bit */
uint32_t clear_first_set_bit(uint32_t value);

/* Position of right most set bit */
uint32_t position_right_most_set_bit(uint32_t value);

/* check the power of two */
uint32_t is_power_of_two(uint32_t value);

/* check the power of four */
uint32_t is_power_of_four(uint32_t n);

/* toggle the bit */
int toggleIthBit(int n, int pos);


#endif
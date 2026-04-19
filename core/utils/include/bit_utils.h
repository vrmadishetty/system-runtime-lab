#ifndef BIT_UTILS_H
#define BIT_UTILS_H

#include <stdint.h>

/* Check if ith bit is set */
uint8_t bit_is_set(uint32_t value, uint8_t pos);

/* set the ith bit */
uint8_t bit_set(uint32_t value, uint8_t pos);


#endif
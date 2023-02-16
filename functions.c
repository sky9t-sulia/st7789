#include "functions.h"

void *memset16(void *m, uint16_t val, size_t count)
{
    uint16_t *buf = m;

    while(count--) *buf++ = val;
    return m;
}

uint16_t swap_bits(uint16_t color) {
    return (color >> 8) | (color << 8);
}
#include "include/config.h"

void delay(uint n)
{
    uint x, y;
    for (x = n; x > 0; x--)
        for (y = 110; y > 0; y--)
            ;
}

void delay_us(uchar n)
{
    while (--n)
        ;
}

void delay_ms(uint z)
{
    uint i, j;
    for (i = z; i > 0; i--)
        for (j = 110; j > 0; j--)
            ;
}
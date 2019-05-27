#include "include/config.h"

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
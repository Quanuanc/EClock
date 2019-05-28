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

void configTimer0() //初始化计数器，定时50ms
{
    TH0 = 0x4C;
    TL0 = 0x00;
    TR0 = 1;
}

void configTimer1()
{
    TH1 = 0x00;
    TL1 = 0x00;
    TR1 = 1;
}

#include "include/config.h"

void delay_us(uchar n) //延时 2*n+2 us
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

// void Delay80us()		//@11.0592MHz
// {
// 	unsigned char i;

// 	i = 34;
// 	while (--i);
// }


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

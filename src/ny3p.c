#include "include/ny3p.h"

void NPlay(uchar n)
{
    N_rst = 1;
    delay_ms(2);
    N_rst = 0;
    delay_ms(2);
    while (n > 0)
    {
        N_data = 1;
        delay_ms(1);
        N_data = 0;
        delay_ms(1);
        n--;
    }
    while (N_busy != 1)
        ; //判断语音模块是否在工作，不工作的时候才可以继续播放下一段
}

void NPlayTimeHour(uchar hour)
{
    uchar ten, unit;
    ten = hour / 10;
    unit = hour % 10;
    if (unit != 0)
    {
        switch (ten)
        {
        case 0:
            NPlay(unit + 1);
            NPlay(13);
            break;
        case 1:
            NPlay(11);
            NPlay(unit + 1);
            NPlay(13);
            break;
        case 2:
            NPlay(3);
            NPlay(11);
            NPlay(unit + 1);
            NPlay(13);
        }
    }
    else
    {
        switch (ten)
        {
        case 0:
            NPlay(1);
            NPlay(13);
            break;
        case 1:
            NPlay(11);
            NPlay(13);
            break;
        default:
            NPlay(3);
            NPlay(11);
            NPlay(13);
        }
    }
}

void NPlayTimeMinute(uchar minute)
{
    uchar ten, unit;
    ten = minute / 10;
    unit = minute % 10;
    if (unit == 0)
    {
        switch (ten)
        {
        case 0:
            NPlay(1);
            NPlay(14);
            break;
        case 1:
            NPlay(11);
            NPlay(14);
            break;
        default:
            NPlay(ten + 1);
            NPlay(11);
            NPlay(14);
        }
    }
    else
    {
        switch (ten)
        {
        case 0:
            NPlay(unit+1);
            NPlay(14);
            break;
        case 1:
            NPlay(11);
            NPlay(unit+1);
            NPlay(14);
            break;
        default:
            NPlay(ten + 1);
            NPlay(11);
            NPlay(unit+1);
            NPlay(14);
        }
    }
}
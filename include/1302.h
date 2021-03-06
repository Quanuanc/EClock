#ifndef _1302_H_
#define _1302_H_

#include<reg52.h>
#include<intrins.h>
#include "config.h"

sbit sclk = P1 ^ 1;
sbit io = P1 ^ 2;
sbit rst = P1 ^ 3;

sbit DS1302_SCLK = P1^1; //DS1302的IO引脚
sbit DS1302_IO = P1^2; //DS1302的通信时钟引脚
sbit DS1302_RST = P1^3; //DS1302使能引脚

void InitDS1302();
uchar DS1302SingleRead(uchar reg);
void DS1302SingleWrite(uchar reg, uchar dat);

#endif
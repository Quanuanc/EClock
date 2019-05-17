#ifndef _1602_H_
#define _1602_H_

#include<reg52.h>
#include<intrins.h>
#include "config.h"


sbit LCD1602_RS = P2^5;
sbit LCD1602_RW = P2^6;
sbit LCD1602_E = P2^7;

#define LCD1602_DB P0

void InitLcd1602();
void LcdWriteCmd(uchar cmd);
void LcdWrite(uchar addr, uchar dat);

#endif
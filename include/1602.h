#include "include/config.h"

sbit LCD1602_RS = P2^5;
sbit LCD1602_RW = P2^6;
sbit LCD1602_E = P2^7;

#define LCD1602_DB P0

void InitLcd1602();
void LcdShowStr(uchar x, uchar y, uchar *str);
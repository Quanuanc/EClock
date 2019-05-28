#ifndef CONFIG_H_
#define CONFIG_H_

#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char
#define uint unsigned int

void delay_us(uchar n);
void delay_ms(uint z);
void configTimer0();
void configTimer1();

#endif
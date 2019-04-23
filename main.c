#include <reg52.h>

sbit Data=P2^7;   //定义DHT11数据线

extern void InitLcd1602();
extern void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);

void main(){
    unsigned char test[]="Hello World";
    InitLcd1602();
    LcdShowStr(0,0,test);
}
